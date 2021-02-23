#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <chrono>
#include <memory>

//CombinedFitPlus:
#include "AlgoFit.h"
#include "Util.h"
#include "FitMaster.h"
#include "MsgLog.h"

//Root:
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TFile.h"
#include "TTree.h"
#include "TMatrixD.h"

using namespace std;

AlgoFit::AlgoFit(FitMaster *fm) : Algo(fm)
{
  fName = "Fit";
  fIter = 0;
  fDumpHistograms = true;
  fDoRaster       = true;
  fIs3D           = false;
  
}

AlgoFit::~AlgoFit()
{
}

void AlgoFit::SetOption(string opt, bool val)
{
  if (opt.compare("do_raster") == 0)  {
    fDoRaster       = val;  
    if (fDoRaster){
      MsgInfo( "Doing raster scan.");
    }
    else{
      MsgInfo( "Not doing raster scan.");
    }
  } else if (opt.compare("dump_hists") == 0) {
    fDumpHistograms = val;
    if (fDumpHistograms){
      MsgInfo( "Histograms will be written into root file.");
    }
    else{
      MsgInfo( "Histograms will NOT be written into root file.");
    }
  } 
  else {
    MsgWarning( "Option "+opt+" not recognized! Ignoring...");
  }
}


void AlgoFit::Run()
{
   //Get new histograms for chi
  MsgDebug(2, "AlgoFit::Run");
  fHchi2       = Get2dHist("chi2",fParName[1],fParName[0],fLogScale[1],fLogScale[0]);
  fHchi2raster = Get2dHist("chi2raster",fParName[1],fParName[0],fLogScale[1],fLogScale[0]);

  // for resonance model now
  if(fPar.size()==3) {
    fIs3D = true;
    if(fDoingRaster==true){
      MsgFatal("Let's not do raster scan for 3D, alright? Maybe you can develop it!");
    }
    fHchi23D   = Get3dHist("chi23D",fParName[1],fParName[0], fParName[2], fLogScale[1],fLogScale[0], fLogScale[2]);
    if(fParName[2]=="Eres") SetParameterNull(fParName[2], 9999999.);
    for(int binX=1; binX<= fHchi2->GetNbinsX(); binX++){
      for(int binY=1; binY<= fHchi2->GetNbinsY(); binY++){
        fHchi2->SetBinContent(binX, binY, 99999999);   // fHchi2 would be the smallest 
      }
    }
  }




  fRasterBestPar.resize(fNbins[fPar.size()-1]);
  fRasterBestChi2.resize(fNbins[fPar.size()-1]);
  for (int i = 0; i < fNbins[fPar.size()-1]; i++) fRasterBestChi2.at(i) = 9999999;

  //Start with null matrix
  fParBestFit=fParNull;
 
  bool converged = false;
  Double_t lastchi2=0;
  ostringstream oss;

  fIter=0;
  fDoingRaster = false; //first do contour; raster needs to be done in second pass
  while (!converged) {
    MsgInfo( MsgLog::Form( "AlgoFit::Run fIter = %d", fIter ));
    if ( fFitMaster->IsLikelihoodFit() || fabs(fMinChi2-lastchi2)<Consts::kChi2ConvergeCrit || fIter>5 ) converged=true;

    for (unsigned int i = 0; i<fParName.size(); i++) 
      fFitMaster->SetParameter(fParName.at(i), fParBestFit.at(i));

    fFitMaster->UpdateHistograms();             // update each MC sample
    fFitMaster->BuildEventVector();             // put every MC sample into a vector(Nue Signal, Nue MC, Numu MC)
    fFitMaster->BuildCollapsedEventVector();    // collapse the vector into collapsed (Nue, Numu)
    fFitMaster->BuildFullErrorMatrix();         // Matrix with 3 blocks (Nue Signal, Nue MC, Numu MC)
    fFitMaster->AddFullOscStat();               // add Nuxx MC event num to diagonal terms
    fFitMaster->BuildCollapsedErrorMatrix();    // collapsed matrix with 2 blocks (Nue, Numu) while signal part is added
    fFitMaster->InvertCollapsedErrorMatrix(); 
    fParLastMatrixUpdate=fParBestFit;
    
    Loop(0);  // loop over all s2t and dm2 pairs and get the chi2
    fIter++;

    for (unsigned int i = 0; i<fParName.size(); i++) 
      fFitMaster->SetParameter(fParName.at(i), fParLastMatrixUpdate.at(i));

    fFitMaster->UpdateHistograms();
    fFitMaster->BuildEventVector();
    fFitMaster->BuildCollapsedEventVector();
    lastchi2 = fFitMaster->GetChi2();
    MsgInfo("-------------------------------------------------------------------------------");
    MsgInfo(MsgLog::Form("Completed Iteration %d",fIter));
    MsgInfo( "Matrix calculated using: ");
    for (unsigned int i = 0; i<fParName.size(); i++) 
      oss <<setw(12)<<fParName.at(i)<<" = "<<setw(10)<<fParLastMatrixUpdate.at(i)<<endl;

    oss<< "Best Fit Parameters: "<<endl;
    for (unsigned int i = 0; i<fParName.size(); i++) 
      oss <<setw(12)<<fParName.at(i)<<" = "<<setw(15)<<fParBestFit.at(i)<<endl;
    oss<< "Min chi2 = "<<fMinChi2<<endl;

    oss << "Chi2 on last iteration = "<<setw(10)<<lastchi2<<endl;
    oss << "Calculated with current matrix at "<<endl;
    for (unsigned int i = 0; i<fParName.size(); i++) 
      oss <<setw(12)<<fParName.at(i)<<" = "<<setw(10)<<fParLastMatrixUpdate.at(i)<<endl;

    oss<<endl;
    MsgInfo(oss.str());
    oss.str("");
  }

  if ( !fFitMaster->IsLikelihoodFit() ) fParBestFit=fParLastMatrixUpdate; //the best fit point 

  
  if (fDoRaster) {
    fDoingRaster = true;
    fIter = 0;
    Loop(0);
  }


  TFile* f    = fFitMaster->GetFoutFake();
  TTree* tree = fFitMaster->GetTreeFake();
  bool isTFileProvided = false;

  if(f!=0 && tree!=0) isTFileProvided = true;

  // -------------------
  // Fake data fit
  // -------------------
  if (fFitMaster->IsFakeDataFit()) {
    if(isTFileProvided==false){
      char* file_name=Form("%s_fake",fFitMaster->GetOutputFileStem().c_str());
      for (UInt_t i=0;i<fParName.size();i++) {
        file_name=Form("%s_%s_%f",file_name,fParName.at(i).c_str(),fFitMaster->GetFakeDataParameter(fParName.at(i)));
      }
      
      if (fFitMaster->GetCurrentFakeDataFitN() == 0) {
        f    =new TFile(Form("%s.root",file_name),"RECREATE");
        tree =new TTree("fit","Fit Results");
      } else {
        f    =new TFile(Form("%s.root",file_name),"UPDATE");
        tree =dynamic_cast<TTree*> (f->Get("fit"));
      }
    }

    if (fDumpHistograms && fHchi2->GetXaxis()->GetNbins()>1) {
      fHchi2->SetName(Form("%s_%i",fHchi2->GetName(),fFitMaster->GetCurrentFakeDataFitN()));
      fHchi2->Write();
    }
    if (fDoRaster) {
      fHchi2raster->SetName(Form("%s_%i",fHchi2raster->GetName(),fFitMaster->GetCurrentFakeDataFitN()));
      fHchi2raster->Write();
    }
    
    fFitMaster->CalculateErrors(fParName, fParNull,"Null",0);  // TEMP
    fFitMaster->CalculateErrors(fParName, fParBestFit,"BF",0); // TEMP

    /* depreciated writing histogram to data
    std::list<std::shared_ptr<Sample>> data_list = fFitMaster->GetListOfSamples("Data");
    for ( auto const& data_it : data_list ) {
      if (fDumpHistograms) data_it->WriteHistogramsToFile(f, true, "Data", Form("%d", fFitMaster->GetCurrentFakeDataFitN()));
    }
    for (unsigned int i = 0; i<fParName.size(); i++) 
      fFitMaster->SetParameter(fParName.at(i), fParBestFit.at(i));

    fFitMaster->UpdateHistograms();
    std::list<std::shared_ptr<Sample>> signal_list = fFitMaster->GetListOfSamples("Signal");
    for ( auto const& signal_it : signal_list ) {
      if (fDumpHistograms) signal_it->WriteHistogramsToFile(f, false, "Signal", Form("%d", fFitMaster->GetCurrentFakeDataFitN()));
    }
    */
  } // if(IsFakeDataFit) 

  // --------------------
  // Not fake data fit
  // --------------------
  else { 

    if(isTFileProvided==false){
      f=new TFile(Form("%s.root",fFitMaster->GetOutputFileStem().c_str()),"RECREATE");
      tree=new TTree("fit","Fit Results");
    }

    // chi2 map
    if (fDumpHistograms){                
      fHchi2->Write();
      if(fIs3D)
        fHchi23D->Write();
    }
    if (fDumpHistograms && fDoRaster)   fHchi2raster->Write();
   
    fFitMaster->CalculateErrors(fParName, fParNull,"Null",f);
    fFitMaster->CalculateErrors(fParName, fParBestFit,"BF",f);

    std::list<std::shared_ptr<Sample>> data_list = fFitMaster->GetListOfSamples("Data");
    for ( auto const& data_it : data_list ) {
      TH1D* h=data_it->GetHistogram();
      if (fDumpHistograms && h!=0) h->Write();
      delete h;
    }
    for (unsigned int i = 0; i<fParName.size(); i++) 
      fFitMaster->SetParameter(fParName.at(i), fParBestFit.at(i));

    fFitMaster->UpdateHistograms();

    std::list<std::shared_ptr<Sample>> signal_list = fFitMaster->GetListOfSamples("Signal");
    for ( auto const& signal_it : signal_list ) {
      signal_it->WriteHistogramsToFile(f);
    }
  } // no fake data


  // -------------
  // Save MC list
  // -------------
  if (fFitMaster->GetCurrentFakeDataFitN()==0) {
    std::list<std::shared_ptr<Sample>> mc_list = fFitMaster->GetListOfSamples("MC");
    for ( auto const& mc_it : mc_list ) {
      if (fDumpHistograms) mc_it->WriteHistogramsToFile(f);
    }
    
    const ErrorMatrix frac_error_matrix=fFitMaster->GetFracErrorMatrix();
    TMatrixD matrix(frac_error_matrix.GetDimX(),frac_error_matrix.GetDimY());
    for (Int_t i=0;i<frac_error_matrix.GetDimX();i++) {
      for (Int_t j=0;j<frac_error_matrix.GetDimY();j++) {
        matrix(i,j)=frac_error_matrix.GetElement(i,j);
      }
    }
    matrix.Write("frac_error_matrix");

    // writing individual error matrix
    vector<string> namesOfMatrices = fFitMaster->GetListOfFracErrorMatrixNames();
    for(auto nameOfMatrix : namesOfMatrices){
      auto individual_frac_error_matrix = fFitMaster->GetFracErrorMatrix(nameOfMatrix);
      TMatrixD matrix2(individual_frac_error_matrix->GetDimX(),individual_frac_error_matrix->GetDimY());
      for (Int_t i=0;i<individual_frac_error_matrix->GetDimX();i++) {
        for (Int_t j=0;j<individual_frac_error_matrix->GetDimY();j++) {
          matrix2(i,j)=individual_frac_error_matrix->GetElement(i,j);
        }
      }
      matrix2.Write((nameOfMatrix).c_str());
    }
  }

  std::vector<Double_t> par_true = fParNull; //null if it is not fake data exp.
  if (fFitMaster->IsFakeDataFit()) {
    for (unsigned int i=0; i < fParName.size(); i++) 
      par_true.at(i) = fFitMaster->GetFakeDataParameter(fParName.at(i));
  }

  Double_t null_chi2_null_matrix             = GetChi2(fParName, fParNull   , fParNull);
  Double_t null_chi2_bf_matrix               = GetChi2(fParName, fParNull   , fParBestFit);
  Double_t null_chi2_truepoint_matrix        = GetChi2(fParName, fParNull   , par_true);
  Double_t bf_chi2_null_matrix               = GetChi2(fParName, fParBestFit, fParNull);
  Double_t bf_chi2_bf_matrix                 = GetChi2(fParName, fParBestFit, fParBestFit);
  Double_t bf_chi2_truepoint_matrix          = GetChi2(fParName, fParBestFit, par_true);
  Double_t truepoint_chi2_null_matrix        = GetChi2(fParName, par_true   , fParNull);
  Double_t truepoint_chi2_bf_matrix          = GetChi2(fParName, par_true   , fParBestFit);
  Double_t truepoint_chi2_truepoint_matrix   = GetChi2(fParName, par_true   , par_true);
  vector<Double_t> fake(fParName.size()); 
  vector<Double_t> bf(fParName.size());
  for (unsigned int i=0; i < fParName.size(); i++) {
    fake.at(i) = par_true.at(i);
    bf.at(i) = fParBestFit.at(i);
  }

  if (fFitMaster->GetCurrentFakeDataFitN()==0) {
    tree->Branch("null_chi2_null_matrix",           &null_chi2_null_matrix,           "null_chi2_null_matrix/D");
    tree->Branch("null_chi2_bf_matrix",             &null_chi2_bf_matrix,             "null_chi2_bf_matrix/D");
    tree->Branch("null_chi2_truepoint_matrix",      &null_chi2_truepoint_matrix,      "null_chi2_truepoint_matrix/D");
    tree->Branch("bf_chi2_null_matrix",             &bf_chi2_null_matrix,             "bf_chi2_null_matrix/D");
    tree->Branch("bf_chi2_bf_matrix",               &bf_chi2_bf_matrix,               "bf_chi2_bf_matrix/D");
    tree->Branch("bf_chi2_truepoint_matrix",        &bf_chi2_truepoint_matrix,        "bf_chi2_truepoint_matrix/D");
    tree->Branch("truepoint_chi2_null_matrix",      &truepoint_chi2_null_matrix,      "truepoint_chi2_null_matrix/D");
    tree->Branch("truepoint_chi2_bf_matrix",        &truepoint_chi2_bf_matrix,        "truepoint_chi2_bf_matrix/D");
    tree->Branch("truepoint_chi2_truepoint_matrix", &truepoint_chi2_truepoint_matrix, "truepoint_chi2_truepoint_matrix/D");
    for (unsigned int i=0; i < fParName.size(); i++) {
      tree->Branch(Form("true_%s",fParName.at(i).c_str()), &fake.at(i), Form("true_%s/D",fParName.at(i).c_str()));
      tree->Branch(Form("bf_%s",fParName.at(i).c_str()), &bf.at(i), Form("bf_%s/D",fParName.at(i).c_str()));
    }
  } else {
    tree->SetBranchAddress("null_chi2_null_matrix",           &null_chi2_null_matrix);
    tree->SetBranchAddress("null_chi2_bf_matrix",             &null_chi2_bf_matrix);
    tree->SetBranchAddress("null_chi2_truepoint_matrix",      &null_chi2_truepoint_matrix);
    tree->SetBranchAddress("bf_chi2_null_matrix",             &bf_chi2_null_matrix);
    tree->SetBranchAddress("bf_chi2_bf_matrix",               &bf_chi2_bf_matrix);
    tree->SetBranchAddress("bf_chi2_truepoint_matrix",        &bf_chi2_truepoint_matrix);
    tree->SetBranchAddress("truepoint_chi2_null_matrix",      &truepoint_chi2_null_matrix);
    tree->SetBranchAddress("truepoint_chi2_bf_matrix",        &truepoint_chi2_bf_matrix);
    tree->SetBranchAddress("truepoint_chi2_truepoint_matrix", &truepoint_chi2_truepoint_matrix);
    for (unsigned int i=0; i < fParName.size(); i++) {
      tree->SetBranchAddress(Form("true_%s",fParName.at(i).c_str()), &fake.at(i));
      tree->SetBranchAddress(Form("bf_%s",fParName.at(i).c_str()), &bf.at(i));
    }
  }

  tree->Fill();
  if(isTFileProvided==false){
    tree->Write(0,TObject::kOverwrite);
    f->Close();
    delete f;
    f=0;
  }

  delete fHchi2;
  delete fHchi2raster;
  if(fIs3D) delete fHchi23D;

  fHchi2=0;
  fHchi2raster=0;
  fHchi23D=0;

}




void AlgoFit::Loop(Int_t n)
{
  MsgDebug(2, "AlgoFit::Loop");
  if (n==0) fMinChi2=1000000;
  //recursive loop that goes through all the parameters 
  Double_t dx;
  if ( fLogScale[n] ) dx = ( log10(fParMax[n]) - log10(fParMin[n]) ) / Double_t(fNbins[n]-1);
  else                dx = ( fParMax[n] - fParMin[n] ) / Double_t(fNbins[n]-1);
  
  for (fBin[n] = 0; fBin[n] < fNbins[n]; fBin[n]++) {
    if ( fLogScale[n] ) fPar[n] = pow(10,log10(fParMin[n])+dx*fBin[n]);
    else                fPar[n] = fParMin[n]+dx*fBin[n];
    fFitMaster->SetParameter(fParName[n], fPar[n]);
    //if(n==0) {
    MsgDebug(1, MsgLog::Form("Setting %13s to bin %3d/%3d at %10.5f", fParName[n].c_str(), fBin[n], fNbins[n], fPar[n]));
    MsgLog::Flush();
    //}

    // Loop over the next variable
    if ( n < ((int)fPar.size()-1) ) {
      Loop(n+1);
    } 
    else 
    {
      // ------------------------------
      // Currently Used One
      if ( !fDoingRaster) { // fDoingRaster == False
        fFitMaster->UpdateHistograms();
        fFitMaster->BuildEventVector();
        fFitMaster->BuildCollapsedEventVector();
        if (fFitMaster->IsLikelihoodFit()) {
          fFitMaster->BuildFullErrorMatrix();
          fFitMaster->AddFullOscStat();
          fFitMaster->BuildCollapsedErrorMatrix();
          fFitMaster->InvertCollapsedErrorMatrix();
        }
        Double_t chi2=fFitMaster->GetChi2();
        if(fIs3D){
          fHchi23D->SetBinContent(fBin.at(1)+1, fBin.at(0)+1, fBin.at(2)+1, chi2);   // the order is to follow the dm2, s2t, Eres order
          Double_t tempChi2 = fHchi2->GetBinContent(fBin.at(1)+1, fBin.at(0)+1);  // in the xml file
          if(tempChi2>chi2){
            fHchi2->SetBinContent(fBin.at(1)+1, fBin.at(0)+1, chi2);              // in 3D mode, 2D is to reflect the minimum among 
          }                                                                 // all different Eres
        }
        else{ // 2D
          fHchi2->SetBinContent(fBin.at(1)+1,fBin.at(0)+1, chi2);
        }

        if (chi2 < fMinChi2) {
          fParBestFit = fPar;
          fMinChi2 = chi2;
        }
        if (fIs3D==false && fIter == 0 && chi2<fRasterBestChi2[fBin.at(n-1)]) {
          fRasterBestPar.at(fBin.at(n-1)) = fPar.at(n);
          fRasterBestChi2.at(fBin.at(n-1)) = chi2;
        }
      } // if(!fDoingRaster)
      // ------------------------------
      else { // if(fDoingRaster) Warning: Depreciated!
        if (fBin.at(n) == 0) {
          fFitMaster->SetParameter(fParName.at(n), fRasterBestPar.at(fBin.at(n-1)) );
          fFitMaster->UpdateHistograms();
          fFitMaster->BuildEventVector();
          fFitMaster->BuildCollapsedEventVector();
          fFitMaster->BuildFullErrorMatrix();
          fFitMaster->AddFullOscStat();
          fFitMaster->BuildCollapsedErrorMatrix();
          fFitMaster->InvertCollapsedErrorMatrix();

          fRasterBestChi2.at(fBin.at(n-1))=fFitMaster->GetChi2();
        }

        fFitMaster->SetParameter(fParName.at(n), fPar.at(n));
        fFitMaster->UpdateHistograms();
        fFitMaster->BuildEventVector();
        fFitMaster->BuildCollapsedEventVector();

        Double_t chi2=fFitMaster->GetChi2();
        fHchi2raster->SetBinContent(fBin[1]+1,fBin[0]+1, chi2 - fRasterBestChi2[fBin.at(n-1)]);
      } // else of if(!fDoingRaster)
    } //else of if ( n < fPar.size()-1 ) 
  } // for (fBin.at(n))
}

Double_t AlgoFit::GetChi2(std::vector<std::string> par_name, std::vector<Double_t> chi2_point, std::vector<Double_t> matrix_point)
{
  MsgDebug(2, "AlgoFit::GetChi2");
  //function returns chi2 calculated at chi2_point using matrix calculated at matrix_point
  for (unsigned int i = 0; i<par_name.size(); i++) 
    fFitMaster->SetParameter(par_name.at(i), matrix_point.at(i));

  fFitMaster->UpdateHistograms();
  fFitMaster->BuildEventVector();
  fFitMaster->BuildCollapsedEventVector();
  fFitMaster->BuildFullErrorMatrix();
  fFitMaster->AddFullOscStat();
  fFitMaster->BuildCollapsedErrorMatrix();
  fFitMaster->InvertCollapsedErrorMatrix(); 
  
  for (unsigned int i = 0; i<par_name.size(); i++) 
    fFitMaster->SetParameter(par_name.at(i), chi2_point.at(i));

  fFitMaster->UpdateHistograms();
  fFitMaster->BuildEventVector();
  fFitMaster->BuildCollapsedEventVector();
  ostringstream oss;
  oss <<"-----------------------------------------------------------------------------------------------"<<endl;
  oss <<setw(12)<<"Parameter"<<setw(16)<<"Chi2 point"<<setw(16)<<"Matrix point"<<endl;
  for (unsigned int i = 0; i<par_name.size(); i++) 
    oss <<setw(12)<<par_name.at(i)<<setw(16)<<chi2_point.at(i)<<setw(16)<<matrix_point.at(i)<<endl;

  oss <<setw(12)<<"chi2   = "<<setw(16)<<fFitMaster->GetChi2()<<endl;
  oss <<setw(12)<<"log(C) = "<<setw(16)<<fFitMaster->GetLogDeterminant()<<endl;
  oss <<endl;
  if(fFitMaster->GetNumberOfFakeDataFits()!=0) MsgInfo(oss.str());
  return fFitMaster->GetChi2();
}
