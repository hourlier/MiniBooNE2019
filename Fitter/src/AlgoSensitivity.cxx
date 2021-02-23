#include "AlgoSensitivity.h"
#include "Util.h"
#include "FitMaster.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include <iostream>
#include <memory>
#include <cmath>

using namespace std;

AlgoSensitivity::AlgoSensitivity(FitMaster *fm) : Algo(fm)
{
  fName = "Sensitivity";
}

AlgoSensitivity::~AlgoSensitivity()
{
}

void AlgoSensitivity::Run()
{
  //1) get null matrix
  //2) loop through parameter space and get chi2

  std::list<std::shared_ptr<Sample>> datalist = fFitMaster->GetListOfSamples("Data");
  std::list<std::shared_ptr<Sample>>::const_iterator data_it=datalist.begin();
  while ( data_it != datalist.end() ) {
    fFitMaster->RemoveDataSample((*data_it)->GetName());
    data_it++;
  }

  std::list<std::shared_ptr<Sample>> mclist = fFitMaster->GetListOfSamples("MC");
  std::list<std::shared_ptr<Sample>>::const_iterator mc_it=mclist.begin();
  while ( mc_it != mclist.end() ) {
    std::shared_ptr<Sample> sensdata=(*mc_it)->Clone();
    TString name(sensdata->GetName());
    name.ReplaceAll("MC","Data");
    sensdata->SetName(name.Data());
    fFitMaster->AddDataSample(sensdata);
    mc_it++;
  }

  fHchi2 = Get2dHist("chi2",fParName[1],fParName[0]);

  //Find the null matrix
  for (unsigned int i = 0; i<fParName.size(); i++) 
    fFitMaster->SetParameter(fParName[i], fParNull[i]);

  fFitMaster->UpdateHistograms();
  fFitMaster->BuildEventVector();
  fFitMaster->BuildCollapsedEventVector();
  fFitMaster->BuildFullErrorMatrix();
  fFitMaster->BuildCollapsedErrorMatrix();
  fFitMaster->InvertCollapsedErrorMatrix(); 

  Loop(0);

  TFile* f=new TFile(Form("%s.root",fFitMaster->GetOutputFileStem().c_str()),"RECREATE");
  fHchi2->Write();
  f->Close();

  //now return the original data
  std::list<std::shared_ptr<Sample>> newdatalist = fFitMaster->GetListOfSamples("Data");
  std::list<std::shared_ptr<Sample>>::const_iterator newdata_it=newdatalist.begin();
  while ( newdata_it != newdatalist.end() ) {
    fFitMaster->RemoveDataSample((*newdata_it)->GetName());
    newdata_it++;
  }
  
  data_it=datalist.begin();
  while ( data_it != datalist.end() ) {
    fFitMaster->AddDataSample(*data_it);
    data_it++;
  }
}

void AlgoSensitivity::Loop(Int_t n)
{
  //recursive loop that goes through all the parameters 
  Double_t dx  = ( log10(fParMax[n]) - log10(fParMin[n]) ) / Double_t(fNbins[n]-1);

  for (fBin[n] = 0; fBin[n] < fNbins[n]; fBin[n]++) {
    fPar[n] = pow(10,log10(fParMin[n])+dx*fBin[n]);
    fFitMaster->SetParameter(fParName[n], fPar[n]);
    if ( n < ((int)fPar.size()-1) ) {
      Loop(n+1);
    } else {
       fFitMaster->UpdateHistograms();
       fFitMaster->BuildEventVector();
       fFitMaster->BuildCollapsedEventVector();

       Double_t chi2=fFitMaster->GetChi2();
       fHchi2->SetBinContent(fBin[1]+1,fBin[0]+1, chi2);
    }
  }
}
