#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <memory>
#include <stdlib.h>

#include <math.h>
#include <time.h>
#include <string>
//Fit stuff
#include "FitMaster.h"
#include "Event.h"
#include "SampleROOT.h"
#include "Consts.h"
//Algo:
#include "AlgoSensitivity.h"
#include "AlgoFit.h"

//WeightCalc:
#include "POTWeightCalc.h"
#include "OscWeightCalc.h"
#include "ExpWeightCalc.h"
#include "ResonanceWeightCalc.h"
#include "ResExtraDimWeightCalc.h"
//Filter:
#include "MsgLog.h"
#include "FuncStopwatch.h"

//root stuff
#include <TROOT.h>
#include "TDatime.h"
#include "TMatrixD.h"
#include "TFile.h"
#include "TTree.h"

using namespace std;

FitMaster* FitMaster::fInstance = NULL;

FitMaster::FitMaster()
{
  //Register algorithms
  std::shared_ptr<Algo> algosens=std::make_shared<AlgoSensitivity>(this);
  Register(algosens);

  std::shared_ptr<Algo> algofit=std::make_shared<AlgoFit>(this);
  Register(algofit);

  //Register Weight Calcs
  std::shared_ptr<WeightCalc> potwc=std::make_shared<POTWeightCalc>();
  Register(potwc);

  std::shared_ptr<WeightCalc> oscwc=std::make_shared<OscWeightCalc>();
  Register(oscwc);

  std::shared_ptr<WeightCalc> expwc=std::make_shared<ExpWeightCalc>();
  Register(expwc);

  std::shared_ptr<WeightCalc> reswc=std::make_shared<ResonanceWeightCalc>();
  Register(reswc);

  std::shared_ptr<WeightCalc> resExtraDimwc=std::make_shared<ResExtraDimWeightCalc>();
  Register(resExtraDimwc);


  //Register Filters

  fRebinVector.clear();
  fEventVector.clear();
  fSumw2Vector.clear();
  fDataVector.clear();
  fBackgroundSignal.clear();
  fMCSampleList.clear();
  fDataSampleList.clear();
  fAddSignalStatError = false;
  fAddStatError = false;
  isTotalRecent = false;
  fOutputFileStem = "output";
  fFakeDataRandomSeed=0;
  fFitType=kLikelihood;
  fLogDeterminant=0;  
  fAddFakeDataToRealData=false;
  fTotPOT=0.;
  fDataPOT=0.;
  fCurrentFakeDataFitN=0;

  foutFake = 0;
  treeFake = 0;
  fReadFromLibrary = false;
  fParNames.clear();
  fParValues.clear();
  fLibraryTicket = "";
  fIncludeLSND = false;

  fTotalFracErrorMatrix = make_shared<ErrorMatrix>();
  fTotalFullErrorMatrix = make_shared<ErrorMatrix>();
  fCollapsedErrorMatrix = make_shared<ErrorMatrix>();
  fInvertedCollapsedErrorMatrix = make_shared<ErrorMatrix>();
}

FitMaster::~FitMaster()
{
}

FitMaster* FitMaster::Instance()
{
  if ( !fInstance ) 
    fInstance = new FitMaster;

  return fInstance;
}

void FitMaster::AddDataSample(std::shared_ptr<Sample> data)
{
  for ( auto const& it_dataSample : fDataSampleList ) {
    if ( it_dataSample->GetName() == data->GetName() ) {
      MsgWarning( "FitMaster::Failed adding "+ data->GetName() +" sample to list of fit samples!");
      MsgWarning( "FitMaster::Sample already in the list.");
      return;
    }
  }
  data->SetVerbosity(fVerbosity);

  fDataSampleList.push_back(data);
  std::vector<Double_t> bins = data->GetBins();
  fDataBins.insert(fDataBins.end(),bins.begin(),bins.end());
data->Dump(); // TEMP
  BuildDataVector();
}

void FitMaster::AddMCSample(std::shared_ptr<Sample> mc)
{
  auto it_MCSample = fMCSampleList.begin();
  while ( it_MCSample != fMCSampleList.end() ) {
    if ( (*it_MCSample)->GetName() == mc->GetName() ) {
      MsgWarning( "FitMaster::Failed adding "+ mc->GetName() +" sample to list of fit samples!" );
      MsgWarning( "FitMaster::Sample already in the list.");
      return;
    }
    it_MCSample++;
  }
  mc->SetVerbosity(fVerbosity);
  fMCSampleList.push_back(mc);
  std::vector<Double_t> bins = mc->GetBins();
  fMCBins.insert(fMCBins.end(),bins.begin(),bins.end());

  //Use names to figure out background and corresponding signal samples
  std::string sample_name = mc->GetName();
  it_MCSample = fMCSampleList.begin();
  while (it_MCSample != fMCSampleList.end() ) {
    std::string temp_name = sample_name;

    // find corresponding MC version for the Signal and save it in fBackgroundSignal as (xxx_MC, xxx_Signal)
    if ( sample_name.find("Signal") != string::npos &&
         temp_name.replace(temp_name.find("Signal"),6,"MC") == (*it_MCSample)->GetName() ) {
      std::pair<std::string, std::string > p((*it_MCSample)->GetName(), sample_name);
      fBackgroundSignal.insert(p);					     
    }
    // find corresponding Signal version for the MC and save it in fBackgroundSignal as (xxx_MC, xxx_Signal
    if ( sample_name.find("MC") != string::npos &&
         temp_name.replace(temp_name.find("MC"),2,"Signal") == (*it_MCSample)->GetName() ) {
      std::pair<std::string, std::string > p(sample_name, (*it_MCSample)->GetName());
      fBackgroundSignal.insert(p);					     
    }
    it_MCSample++;
  }
}

void FitMaster::BuildEventVector()
// put every bin content into fEventVector
{
  MsgDebug(2, " ");

  fEventVector.clear();
  fSumw2Vector.clear();

  if(fReadFromLibrary==true){
    MsgDebug(2, "skipping BuildEventVector");
    return;
  }


  auto it_MCSample = fMCSampleList.begin();
  while ( it_MCSample != fMCSampleList.end() ) {
    for (Int_t i=0; i<(*it_MCSample)->GetNbins();i++ ) {
      fEventVector.push_back((*it_MCSample)->GetBinContent(i));
      fSumw2Vector.push_back((*it_MCSample)->GetBinSumw2(i));
    }
    
    it_MCSample++;
  }
}

void FitMaster::BuildDataVector()
{
  fDataVector.clear();
  for ( auto const& it_dataSample : fDataSampleList ) {
    for (Int_t i=0; i<it_dataSample->GetNbins();i++ ) {
      fDataVector.push_back(it_dataSample->GetBinContent(i));
    }
  }
  fNdataBins=fDataVector.size();
}

void FitMaster::BuildCollapsedEventVector()
{
  /*
    function loops through list of mc samples
    at first it ignores samples named "Signal ...." and adds them only when it finds 
    associated background sample, e.g.
    Add Nuebar Signal when loop through Nuebar MC (fBackgroundSignal["Nuebar MC"] = "Nuebar Signal")
    Add Numubar MC separately (there is no entry for Numubar MC in fBackgroundSignal)
  */
  MsgDebug(2, " ");

  // reading from library for fake data
  if(fReadFromLibrary==true){
    fCollapsedEventVector = fCollapsedEventVectorLibrary[fLibraryTicket];
    return;
  }


  fCollapsedEventVector.clear();
  for ( auto const& it_MCSample : fMCSampleList ) {
    if ( (it_MCSample->GetName()).find("Signal") == std::string::npos ) {
      if ( fBackgroundSignal.find(it_MCSample->GetName()) != fBackgroundSignal.end() ) {
        list<std::shared_ptr<Sample> >::const_iterator it_Signal = fMCSampleList.begin();
        while ( (*it_Signal)->GetName() != fBackgroundSignal[it_MCSample->GetName()] ) it_Signal++;
        for (Int_t i=0; i<it_MCSample->GetNbins();i++ ) {
          fCollapsedEventVector.push_back(it_MCSample->GetBinContent(i)+(*it_Signal)->GetBinContent(i));
        }
      } 
      else {
        for (Int_t i=0; i<it_MCSample->GetNbins();i++ ) {
          fCollapsedEventVector.push_back(it_MCSample->GetBinContent(i));
        }
      }
    } // 
  }

  // for fake data
  if(fNfakeDataFits>0)
    fCollapsedEventVectorLibrary[fLibraryTicket] = fCollapsedEventVector;


}


 void FitMaster::AddFracErrorMatrix(std::shared_ptr<ErrorMatrix> matrix)
 {
  map<std::string, std::shared_ptr<ErrorMatrix> >::const_iterator it = fFracErrorMatrix.find(matrix->GetName());
  if ( it != fFracErrorMatrix.end() ) {
    MsgWarning( "FitMaster::Failed adding "+ matrix->GetName() +" matrix to list of matrices!");
    return;
  }
  matrix->SetVerbosity(fVerbosity);
  pair<std::string, std::shared_ptr<ErrorMatrix> > p(matrix->GetName(), matrix);
  fFracErrorMatrix.insert(p);

  isTotalRecent = false;
}

void FitMaster::RecalculateTotalFracErrorMatrix() 
{
  map<std::string, std::shared_ptr<ErrorMatrix> >::iterator it = fFracErrorMatrix.begin();
  if ( it == fFracErrorMatrix.end() ) {
    MsgWarning( "FitMaster::Failed creating total fractional error matrix. " );
    return;
  }
  Int_t dimx = it->second->GetDimX();  Int_t dimy = it->second->GetDimY();


  fTotalFracErrorMatrix->Resize(dimx,dimy);
  fTotalFracErrorMatrix->Clear();
  fTotalFracErrorMatrix->SetName("Total Fractional Error Matrix");
  fTotalFracErrorMatrix->SetVerbosity(fVerbosity);

  while ( it != fFracErrorMatrix.end() ) {
    MsgInfo( "Adding " + it->first);
    (it->second)->MakePosDef(true); // don't make positive definite of each matrix, only run as dryrun
    *fTotalFracErrorMatrix = *fTotalFracErrorMatrix + *(it->second);
    it++;
  }
  fTotalFracErrorMatrix->MakePosDef(true); // dryrun. Only checking
  isTotalRecent = true;
}

void FitMaster::BuildFullErrorMatrix()
{
  MsgDebug(2, " ");
  StartWatch("FitMaster::BuildFullErrorMatrix");

  if(fReadFromLibrary){
    MsgDebug(2, "fReadFromLibrary");
    //fTotalFullErrorMatrix = fCollapsedErrorMatrixLibrary[fLibraryTicket];
    StopWatch("FitMaster::BuildFullErrorMatrix");

    return;
  }

  if ( ((int) fEventVector.size()) != fTotalFracErrorMatrix->GetDimX() ) {
    MsgFatal("FitMaster::Can't build full error matrix. Size of event vector does not match matrix size.");
    exit(1);
  }

  Int_t dimx = fTotalFracErrorMatrix->GetDimX();
  Int_t dimy = fTotalFracErrorMatrix->GetDimY();
  if(dimx != dimy){
    MsgFatal(MsgLog::Form("FitMaster::dimx!=dimy! %d!=%d",dimx, dimy));
    exit(1);
  }
  fTotalFullErrorMatrix = make_shared<ErrorMatrix>();
  fTotalFullErrorMatrix->Resize(dimx,dimy);
  fTotalFullErrorMatrix->Clear();
  fTotalFullErrorMatrix->SetName("Total Full Error Matrix");
  fTotalFullErrorMatrix->SetVerbosity(fVerbosity);

  for ( Int_t i=0;i<dimx;i++) {
    for ( Int_t j=0;j<dimy;j++) {
      Double_t mij = fTotalFracErrorMatrix->GetElement(i,j) * fEventVector.at(i) * fEventVector.at(j);
      if ( mij == mij )
       fTotalFullErrorMatrix->SetElement(i,j, mij);
      else {
       MsgWarning(MsgLog::Form("Warning::Element (%i, %i) in Full error matrix is nan. Setting it to 0", i, j));
       fTotalFullErrorMatrix->SetElement(i,j, 0.);
     }
   }
 }

//fTotalFullErrorMatrix->MakePosDef(true);
if(fNfakeDataFits>0){
  //if ( fTotalFullErrorMatrixLibrary.find(fLibraryTicket) != fTotalFullErrorMatrixLibrary.end() )
  //  MsgWarning( "Replacing "+ fLibraryTicket +" in fTotalFullErrorMatrixLibrary");
  //fTotalFullErrorMatrixLibrary[fLibraryTicket] = fTotalFullErrorMatrix;
}

StopWatch("FitMaster::BuildFullErrorMatrix");

}

/*
void FitMaster::BuildCollapsedErrorMatrix()
{
  
  //  takes full error matrix, splits it into blocks (matrix_blocks) corresponding to MC sample list
  //  ie (fullosc, fullosc), (fullosc, nue), (fullosc, numu),... 
  //  or ie (Nuebar MC,Nuebar MC), (Nuebar MC,Nuebar Signal), (Nuebar MC,Numubar MC)
  //  add non signal blocks and appropriate signal blocks
  //  finally assemble the blocks into collapsed matrix

  //first separate blocks from the full error matrix
  MsgDebug(2, " ");
  StartWatch("FitMaster::BuildCollapsedErrorMatrix");
  if(fReadFromLibrary){
    MsgDebug(2, "fReadFromLibrary");
    //fCollapsedErrorMatrix = fCollapsedErrorMatrixLibrary[fLibraryTicket];
    fLogDeterminant=fLogCollapsedErrorMatrixLibrary[fLibraryTicket];
    StopWatch("FitMaster::BuildCollapsedErrorMatrix");
    return;
  }

  // matrix_blocks consist of block matrices like (Nue Signal, Nue Signal), (Nue Signal, Nue MC), (Nue Signal, Numu Signal)
  // (Nue Signal, Nuebar Signal), (Nue Signal, Nuebar MC), (Nue Signal, Numubar MC) ......
  // If name doesn't have "Signal", then sum the number of bins for collapsed 
  std::map<std::string, std::map<std::string, ErrorMatrix> > matrix_blocks;
  Int_t big_i_off=0;
  Int_t big_j_off=0;
  Int_t n_bins_collapsed=0;
  for ( auto const& itx : fMCSampleList ) {
    big_j_off=0;
    for ( auto const& ity : fMCSampleList ) { 
      ErrorMatrix block;
      Int_t nx = itx->GetNbins();
      Int_t ny = ity->GetNbins();
      block.Resize(nx,ny);
      block.SetName(itx->GetName()+","+ity->GetName());
      for (Int_t i=0;i<nx;i++) {
        for (Int_t j=0;j<ny;j++) {
          block.SetElement(i,j,fTotalFullErrorMatrix->GetElement(i+big_i_off, j+big_j_off));
        }
      }
      std::pair<string, ErrorMatrix> p1(ity->GetName(), block);

      std::map<std::string, std::map<std::string, ErrorMatrix> >::iterator mb_it = matrix_blocks.find(itx->GetName());
      if ( mb_it == matrix_blocks.end() ) {
        std::map<string, ErrorMatrix > m1;
        m1.insert(p1);
        std::pair<string, std::map<string, ErrorMatrix> > p2(itx->GetName(), m1);     
        matrix_blocks.insert(p2);
      } else {
        (mb_it->second).insert(p1);
      }
      big_j_off += ity->GetNbins();
    } // while(ity)
    big_i_off += itx->GetNbins();
    if ( (itx->GetName()).find("Signal") == std::string::npos ) // only sum the bins of non signal
      n_bins_collapsed += itx->GetNbins();
  } // while (itx)
 

  fCollapsedErrorMatrix = make_shared<ErrorMatrix>();
  fCollapsedErrorMatrix->Resize(n_bins_collapsed, n_bins_collapsed);
  fCollapsedErrorMatrix->SetName("Collapsed Error Matrix");
  fCollapsedErrorMatrix->SetVerbosity(fVerbosity);

  //now add non-signal blocks and appropriate signal blocks, e.g.
  // if we got Nuebar Signal, Nuebar MC, Numubar MC, then
  // block(Nuebar MC, Nuebar MC) +=  block(Nuebar Signal, Nuebar MC) + block(Nuebar MC, Nuebar Signal) + block(Nuebar Signal, Nuebar Signal)
  // block(Nuebar MC, Numubar MC) +=  block(Nuebar Signal, Numubar MC) 
  // block(Numubar MC, Nuebar MC) +=  block(Numubar MC, Nuebar Signal)

  std::map<std::string, std::map<std::string, ErrorMatrix> >::const_iterator mbout_it = 
  matrix_blocks.begin();
  while (mbout_it != matrix_blocks.end() ) {
    std::map<std::string, ErrorMatrix>::const_iterator mbin_it = (mbout_it->second).begin();
    while (mbin_it != (mbout_it->second).end() ) {

      if ( (mbout_it->first).find("Signal") == std::string::npos && 
           (mbin_it->first).find("Signal") == std::string::npos ) 
      {
        if (fBackgroundSignal.find(mbout_it->first) != fBackgroundSignal.end() && 
            fBackgroundSignal.find(mbin_it->first)  != fBackgroundSignal.end()) 
        {
          matrix_blocks[mbout_it->first][mbin_it->first]=
                  matrix_blocks[mbout_it->first][mbin_it->first]+
                  matrix_blocks[fBackgroundSignal[mbout_it->first]][fBackgroundSignal[mbin_it->first]];
          matrix_blocks[mbout_it->first][mbin_it->first]=
                  matrix_blocks[mbout_it->first][mbin_it->first]+
                  matrix_blocks[fBackgroundSignal[mbout_it->first]][mbin_it->first];
          matrix_blocks[mbout_it->first][mbin_it->first]=
                  matrix_blocks[mbout_it->first][mbin_it->first]+
                  matrix_blocks[mbout_it->first][fBackgroundSignal[mbin_it->first]];
        } 
        else if (fBackgroundSignal.find(mbout_it->first) == fBackgroundSignal.end() && 
                 fBackgroundSignal.find(mbin_it->first)  != fBackgroundSignal.end() ) 
        {
          matrix_blocks[mbout_it->first][mbin_it->first]=
                  matrix_blocks[mbout_it->first][mbin_it->first]+
                  matrix_blocks[mbout_it->first][fBackgroundSignal[mbin_it->first]];
        } 
        else if (fBackgroundSignal.find(mbout_it->first) != fBackgroundSignal.end() && 
                 fBackgroundSignal.find(mbin_it->first)  == fBackgroundSignal.end()) 
        {
          matrix_blocks[mbout_it->first][mbin_it->first]=
                  matrix_blocks[mbout_it->first][mbin_it->first]+
                  matrix_blocks[fBackgroundSignal[mbout_it->first]][mbin_it->first];
        }
      } // if(mbout_it no signal, mbin_it no signal)
      mbin_it++;
    }
    mbout_it++;
  }

//combine all blocks into a collapsed error matrix
Int_t collapsed_i=0;
for ( auto const& sample_it1 : fMCSampleList ) {
  if ( sample_it1->GetName().find("Signal") == std::string::npos ) {
    Int_t collapsed_j=0;
    for ( auto const& sample_it2 : fMCSampleList ) {
      if ( sample_it2->GetName().find("Signal") == std::string::npos ) {
        unsigned int mnx= matrix_blocks[sample_it1->GetName()][sample_it2->GetName()].GetDimX();
        unsigned int mny= matrix_blocks[sample_it1->GetName()][sample_it2->GetName()].GetDimY();
        ErrorMatrix* thisblock = &matrix_blocks[sample_it1->GetName()][sample_it2->GetName()];
        for (unsigned int i=0; i<mnx; i++) {
          for (unsigned int j=0; j<mny; j++) {
            Double_t mij=(thisblock->GetElement(i,j));
            fCollapsedErrorMatrix->SetElement(collapsed_i+i,collapsed_j+j, mij);
          }
        }
        collapsed_j += sample_it2->GetNbins();
      }
    } // for(sample_it2:fMCSampleList)
    collapsed_i += sample_it1->GetNbins();
  }
} // for(sample_it:fMCSampleList)

StartWatch("FitMaster::BuildCollapsedErrorMatrix2");

if (fAddStatError) AddStatError();
if (fFitType == kLikelihood) 
  fLogDeterminant=log(fCollapsedErrorMatrix->GetDeterminant());

StopWatch("FitMaster::BuildCollapsedErrorMatrix2");

//fCollapsedErrorMatrix->MakePosDef(true); // only run as a dry run. Currently disabled

// save for fake data
if(fNfakeDataFits>0){
  //if ( fCollapsedErrorMatrixLibrary.find(fLibraryTicket) != fCollapsedErrorMatrixLibrary.end() )
  //  MsgWarning( "Replacing "+ fLibraryTicket +" in fCollapsedErrorMatrixLibrary");
  //fCollapsedErrorMatrixLibrary[fLibraryTicket] = fCollapsedErrorMatrix;
  fLogCollapsedErrorMatrixLibrary[fLibraryTicket] = log(fCollapsedErrorMatrix->GetDeterminant());

}

StopWatch("FitMaster::BuildCollapsedErrorMatrix");


} */

void FitMaster::BuildCollapsedErrorMatrix()
{
  /*
    takes full error matrix, splits it into blocks (matrix_blocks) corresponding to MC sample list
    ie (fullosc, fullosc), (fullosc, nue), (fullosc, numu),... 
    or ie (Nuebar MC,Nuebar MC), (Nuebar MC,Nuebar Signal), (Nuebar MC,Numubar MC)
    add non signal blocks and appropriate signal blocks
    finally assemble the blocks into collapsed matrix
  */

  //first separate blocks from the full error matrix
  MsgDebug(2, " ");
  StartWatch("FitMaster::BuildCollapsedErrorMatrix");
  if(fReadFromLibrary){
    MsgDebug(2, "fReadFromLibrary");
    //fCollapsedErrorMatrix = fCollapsedErrorMatrixLibrary[fLibraryTicket];
    fLogDeterminant=fLogCollapsedErrorMatrixLibrary[fLibraryTicket];
    StopWatch("FitMaster::BuildCollapsedErrorMatrix");
    return;
  }

// find the correct bin range
map<string, vector<int> > sampleBinRanges;  // e.g., ["Nue MC", [10, 18]]
int n_bins_full      = 0;
int n_bins_collapsed = 0;
for(auto const& MCSample : fMCSampleList){
  string sampleName    = MCSample->GetName();
  int nbins            = MCSample->GetNbins();
  vector<int> binRange = {n_bins_full, n_bins_full + nbins - 1};
  sampleBinRanges.insert(make_pair(sampleName, binRange));

  n_bins_full += nbins;
  if(sampleName.find("Signal")==string::npos) n_bins_collapsed += nbins;
}

// create collapsedErrorMatrix is not created already
if(fCollapsedErrorMatrix==NULL){
  fCollapsedErrorMatrix = make_shared<ErrorMatrix>();
}
if(fCollapsedErrorMatrix->GetDimX()!= n_bins_collapsed || fCollapsedErrorMatrix->GetDimY()!= n_bins_collapsed){
  fCollapsedErrorMatrix->Resize(n_bins_collapsed, n_bins_collapsed);
  fCollapsedErrorMatrix->SetName("Collapsed Error Matrix");
  fCollapsedErrorMatrix->SetVerbosity(fVerbosity);
}
else fCollapsedErrorMatrix->Reset();

//combine all blocks into a collapsed error matrix
int iXcollapsed=0;
int iXfull = 0, iYfull = 0, nBinsX=0, nBinsY=0;
vector< pair<string, string> > blockNames2Sum;    // names of (SampleX, SampleY) to sum
for ( auto const& MCSampleX : fMCSampleList ) {
  // will add signal in the turn of the corresponding "MC"
  if ( MCSampleX->GetName().find("Signal") != std::string::npos ) continue;
  int iYcollapsed=0;
  for ( auto const& MCSampleY : fMCSampleList ) {
    if ( MCSampleY->GetName().find("Signal") != std::string::npos ) continue;

    string sampleNameX = MCSampleX->GetName();
    string sampleNameY = MCSampleY->GetName();
    blockNames2Sum.clear();

    // 0. Add the current block
    blockNames2Sum.push_back(make_pair(MCSampleX->GetName(), MCSampleY->GetName()) ); 

    // 1. If both MCSampleX & MCSampleY have the corresponding Signal 
    if(fBackgroundSignal.find(sampleNameX)!=fBackgroundSignal.end() && 
       fBackgroundSignal.find(sampleNameY)!=fBackgroundSignal.end()){
      
      blockNames2Sum.push_back(make_pair(fBackgroundSignal[sampleNameX] , fBackgroundSignal[sampleNameY]));  // Signal, Signal
      blockNames2Sum.push_back(make_pair(fBackgroundSignal[sampleNameX] , sampleNameY                   ));  // Signal, MC
      blockNames2Sum.push_back(make_pair(sampleNameX                    , fBackgroundSignal[sampleNameY]));  // MC    , Signal
    }
    // 2. If only MCSampleX has corresponding Signal
    else if(fBackgroundSignal.find(sampleNameX)!=fBackgroundSignal.end() && 
            fBackgroundSignal.find(sampleNameY)==fBackgroundSignal.end()){
      blockNames2Sum.push_back(make_pair(fBackgroundSignal[sampleNameX] , sampleNameY                   ));  // Signal, MC
    }

    // 3. If only MCSampleY has corresponding Signal
    else if(fBackgroundSignal.find(sampleNameX)==fBackgroundSignal.end() && 
            fBackgroundSignal.find(sampleNameY)!=fBackgroundSignal.end()){
      blockNames2Sum.push_back(make_pair(sampleNameX                    , fBackgroundSignal[sampleNameY]));  // MC    , Signal
    }
    
    // sum over the blocks
    for(auto const& blockName : blockNames2Sum){
      iXfull = sampleBinRanges[blockName.first ].at(0);              // initial position in fullErrorMatrix for copy
      iYfull = sampleBinRanges[blockName.second].at(0);              // initial position in fullErrorMatrix for copy
      nBinsX = sampleBinRanges[blockName.first ].at(1) - iXfull + 1; // nbinsX to copy
      nBinsY = sampleBinRanges[blockName.second].at(1) - iYfull + 1; // nbinsY to copy
      fCollapsedErrorMatrix->AddBlock2Block(fTotalFullErrorMatrix, iXfull, iYfull, iXcollapsed, iYcollapsed, nBinsX, nBinsY);
    }

    iYcollapsed += MCSampleY->GetNbins();
  } // for(MCSampleY:fMCSampleList)
  iXcollapsed += MCSampleX->GetNbins();
} // for(sample_it:fMCSampleList)

StartWatch("FitMaster::BuildCollapsedErrorMatrix2");

if (fAddStatError) AddStatError();
if (fFitType == kLikelihood) 
  fLogDeterminant=log(fCollapsedErrorMatrix->GetDeterminant());

StopWatch("FitMaster::BuildCollapsedErrorMatrix2");

//fCollapsedErrorMatrix->MakePosDef(true); // only run as a dry run. Currently disabled

// save for fake data
if(fNfakeDataFits>0){
  //if ( fCollapsedErrorMatrixLibrary.find(fLibraryTicket) != fCollapsedErrorMatrixLibrary.end() )
  //  MsgWarning( "Replacing "+ fLibraryTicket +" in fCollapsedErrorMatrixLibrary");
  //fCollapsedErrorMatrixLibrary[fLibraryTicket] = fCollapsedErrorMatrix;
  fLogCollapsedErrorMatrixLibrary[fLibraryTicket] = log(fCollapsedErrorMatrix->GetDeterminant());

}

StopWatch("FitMaster::BuildCollapsedErrorMatrix");


}


void FitMaster::InvertCollapsedErrorMatrix() { 
  MsgDebug(2, " ");
  StartWatch("FitMaster::InvertCollapsedErrorMatrix");
  if(fReadFromLibrary){
    MsgDebug(2, "fReadFromLibrary");
    fInvertedCollapsedErrorMatrix = fInvertedCollapsedErrorMatrixLibrary[fLibraryTicket];
    StopWatch("FitMaster::InvertCollapsedErrorMatrix");
    return;
  }

  fInvertedCollapsedErrorMatrix = make_shared<ErrorMatrix>();
  fInvertedCollapsedErrorMatrix->CopyFrom(fCollapsedErrorMatrix);
  fInvertedCollapsedErrorMatrix->Invert();
  fInvertedCollapsedErrorMatrix->SetName("Inverted Collapsed Error Matrix");

  // save for fake data
  if(fNfakeDataFits>0){
    //if ( fInvertedCollapsedErrorMatrixLibrary.find(fLibraryTicket) != fInvertedCollapsedErrorMatrixLibrary.end() )
    //  MsgWarning( "Replacing "+ fLibraryTicket +" in fInvertedCollapsedErrorMatrixLibrary");
    fInvertedCollapsedErrorMatrixLibrary[fLibraryTicket] = fInvertedCollapsedErrorMatrix;
  }
  StopWatch("FitMaster::InvertCollapsedErrorMatrix");

};

void FitMaster::Rebin(std::map<std::string, std::vector<Int_t> > rebin_map)
{  
  /*
    Rebins event vectors, fractional and full error matrix according to vectors in rebin_map
  */

  /* First create a rebin vector for matrices
     If rebin to 0, write it to 0. If not 0, write it to bin number + offset of previous bins.
     eg. vec1 = 1, 1, 2, 3   ; vec2 = 0, 1, 2, 0
     full_rebin_vector = 1, 1, 2, 3, 0, 4, 5, 0
  */
  MsgInfo("*********************************************************************************");
  MsgInfo("FitMaster::Rebin");
  StartWatch("FitMaster::Rebin");
  ostringstream oss;
  fRebinMap=rebin_map;
  std::vector<Int_t> full_rebin_vector;
  Int_t bin_offset = 0;
  Int_t n_bins_new = 0;
  for ( auto const& ds_it : fMCSampleList ) {
    map<string, std::vector<Int_t> >::const_iterator rebinmap_it  = rebin_map.find(ds_it->GetName());
    if ( rebinmap_it == rebin_map.end() ) {
      MsgError("FitMaster::Fatal Error!");
      MsgFatal("FitMaster::Rebin map does not have new bins for "+ds_it->GetName() +" sample");
      exit (0);
    }
    MsgInfo(rebinmap_it->first);
    for (unsigned int i=0; i<(rebinmap_it->second).size();i++) {
      oss << rebinmap_it->second.at(i) << ", ";
      if ( rebinmap_it->second.at(i) != 0 ) { // if not bin0, then put the (absolute of the bin number + offset to the end)
        full_rebin_vector.push_back(abs((rebinmap_it->second).at(i))+bin_offset); 
      }
      else {
        full_rebin_vector.push_back(abs((rebinmap_it->second).at(i))); // if bin0, then just attach it.
      }
    }
    MsgInfo(oss.str());
    oss.str("");
    Int_t rewind = full_rebin_vector.size();
    bin_offset   = full_rebin_vector.back();
    while ( bin_offset == 0 && rewind>0 ) {bin_offset   = full_rebin_vector.at(rewind-1); rewind--; } // if last one was 0, then move to previous one
    n_bins_new = bin_offset;
  } // for(ds_it:fMCSampleList)
  
  // print full rebin vector

  oss << "full_rebin_vector" << endl;
  for(unsigned int i = 0; i<full_rebin_vector.size(); i++){
    oss << full_rebin_vector.at(i) << ", ";
  }
  MsgInfo(oss.str());
  oss.str("");

  //need to loop over MC samples and turn off weight calcs;
  for ( auto const& ds_it : fMCSampleList ) {
    std::vector<std::shared_ptr<WeightCalc>> wcalc = ds_it->GetWeightCalc();
    for (unsigned int i=0; i<wcalc.size();i++) wcalc[i]->TurnOff();
    //also need to force refilling of histograms
    ds_it->FillHistogram();
  }


  //use full error matrix to rebin 
  BuildEventVector();
  BuildFullErrorMatrix();
  //Int_t rewind = full_rebin_vector.size();

  // Rebin error matrix: sum all the ones within the matrix
  std::vector<std::vector<Double_t> > new_matrix;
  new_matrix.resize(n_bins_new);
  for (int i=0; i<n_bins_new; i++) new_matrix.at(i).resize(n_bins_new);

  for (Int_t i=0;i<fTotalFullErrorMatrix->GetDimX();i++) {
    for (Int_t j=0;j<fTotalFullErrorMatrix->GetDimY();j++) {
      if (full_rebin_vector.at(i) < 0 || full_rebin_vector.at(j) < 0) {
       MsgError( "FitMaster::Can't rebin! Invalid value in rebin vector." );
       return;
      }
      Int_t new_i = full_rebin_vector.at(i)-1;
      Int_t new_j = full_rebin_vector.at(j)-1;

      if (full_rebin_vector.at(i) != 0 && full_rebin_vector.at(j) !=0 ) {
        new_matrix.at(new_i).at(new_j) += fTotalFullErrorMatrix->GetElement(i,j);
      }
    }    
  }

  //now rebin all the samples and rebuild event vector
  fMCBins.clear();
  for ( auto const& ds_it : fMCSampleList ) {
    ds_it->Rebin(rebin_map[ds_it->GetName()]);
    std::vector<Double_t> bins = ds_it->GetBins();
    fMCBins.insert(fMCBins.end(),bins.begin(),bins.end());
  }
  BuildEventVector();  // build MC event vector : fEventVector & fSumw2Vector

  fDataBins.clear();
  for ( auto const& data_it : fDataSampleList ) {
    data_it->Rebin(rebin_map[data_it->GetName()]);
    std::vector<Double_t> bins = data_it->GetBins();
    fDataBins.insert(fDataBins.end(),bins.begin(),bins.end());
  }
  BuildDataVector();   // build data event vector : fDataVector

  //finally rebuild the matrices
  fTotalFullErrorMatrix->Clear();
  fTotalFullErrorMatrix->Resize(n_bins_new,n_bins_new);
  fTotalFracErrorMatrix->Clear();
  fTotalFracErrorMatrix->Resize(n_bins_new,n_bins_new);
  for (Int_t i=0;i<fTotalFullErrorMatrix->GetDimX();i++) {
    for (Int_t j=0;j<fTotalFullErrorMatrix->GetDimY();j++) {
      fTotalFullErrorMatrix->SetElement(i,j,new_matrix.at(i).at(j));
      fTotalFracErrorMatrix->SetElement(i,j,new_matrix.at(i).at(j)/(fEventVector.at(i)*fEventVector.at(j)));
    }
  }

  //now turn back on weights and refill histograms again;
  for ( auto const& ds_it : fMCSampleList ) {
    std::vector<std::shared_ptr<WeightCalc>> wcalc = ds_it->GetWeightCalc();
    for (unsigned int i=0; i<wcalc.size();i++) wcalc[i]->TurnOn();
      ds_it->FillHistogram();
  }

  BuildCollapsedEventVector();
  BuildCollapsedErrorMatrix();

  StopWatch("FitMaster::Rebin");


}

  void FitMaster::UpdateHistograms()
  {
  /*
    Loop through all the samples and update them if any of the parameters changed
  */
    MsgDebug(2, " ");
    StartWatch("FitMaster::UpdateHistograms");

    if(fReadFromLibrary==true){
      MsgDebug(2, "skip updateHistogram. read from library.");
      StopWatch("FitMaster::UpdateHistograms");
      return;
    }
    for(auto& MCSample : fMCSampleList) MCSample->Update();
    //for_each( fMCSampleList.begin()  , fMCSampleList.end()  , mem_fun(&Sample::Update));
    //for_each( fDataSampleList.begin(), fDataSampleList.end(), mem_fun(&Sample::Update));
    StopWatch("FitMaster::UpdateHistograms");

  }

  void FitMaster::CorrectMSstat(std::string matrix_name, std::map<std::string, Double_t> ms_to_data_pot)
  {
  /*
    This function corrects the diagonal elements of the fractional
    error matrix to account for the differences between the 
    central value in the MC and in the Multisims. Also it accounts
    for potentially different POT used to generate multisims.
    It does not apply correction to fullosc bins.
    It assumes that the fracM_CV has already been scaled to data pot.
  */
    MsgDebug(2, MsgLog::Form("matrix %s", matrix_name.c_str()));
    // test whether matrix exist
    if ( fFracErrorMatrix.find(matrix_name) == fFracErrorMatrix.end() ) {
      MsgError("FitMaster::Can't correct diagonal elements for "+matrix_name+". Matrix not loaded.");
      return;
    }

    std::vector<Double_t> fracM_CV = fFracErrorMatrix[matrix_name]->GetCV();
    int nbins = (int) fEventVector.size();

    MsgInfo("FitMaster::Correcting diagonal elements of "+matrix_name);
    ostringstream oss;
    std::list<std::shared_ptr<Sample>>::const_iterator mc_it=fMCSampleList.begin();
    int nBinsChangeLine = (*mc_it)->GetNbins(); // to print CV by each sample name

    // Print Multisim CV
    MsgInfo("FitMaster::Multisim cv:");
    oss << setw(15) << (*mc_it)->GetName();
    for (int i=0; i<nbins; i++) 
    {
      oss<<setw(9) <<setprecision(5) <<fracM_CV.at(i);
      if(i==nBinsChangeLine-1){
        mc_it++;
        if(mc_it != fMCSampleList.end()){
          nBinsChangeLine += (*mc_it)->GetNbins();
          oss << endl << setw(15) << (*mc_it)->GetName();
        }
      }
    }
    oss << endl << endl;

    int ibinFullEvent = 0;
    oss<<"FitMaster::MC cv:"<<endl;
    for(auto const& MCSample : fMCSampleList){
      oss << setw(15) << MCSample->GetName();
      for(int ibin=0; ibin<MCSample->GetNbins(); ibin++){
        oss << setw(10) << setprecision(7) << fEventVector.at(ibinFullEvent);
        ibinFullEvent++;
      }
      oss << endl;
    }
    oss << endl << endl;
    MsgInfo(oss.str());
    oss.str("");

    // Start correction
    mc_it=fMCSampleList.begin();
    Int_t bin = 0;

    oss << "FitMaster::Correcting mattrix elements" << endl;
    oss << "bin \t FracM \t EventVector \t        FracM_CV_POTscaled \t\t FracM_scaled" << endl;
    while ( mc_it != fMCSampleList.end() ) {
      // ignore signals
      if ( ((*mc_it)->GetName()).find("Signal") != std::string::npos ) {
        bin += (*mc_it)->GetNbins();
      } 
      // not signals
      else {
        oss << "------------------------------- " << (*mc_it)->GetName() << " -------------------------------" << endl;
        for (int i=0; i<(*mc_it)->GetNbins(); i++) {
          Double_t fracM_ii = fFracErrorMatrix[matrix_name]->GetElement(bin,bin);

          Double_t evntV_i = fEventVector.at(bin);
          //undo scaling of multisim to data pot
          Double_t fracM_CV_POT = fracM_CV.at(bin)/ms_to_data_pot[(*mc_it)->GetName()];

          oss << Form("%3d %10.6f %10.4f %13.4f/%6.4f=%11.4f", 
                       bin, fracM_ii, evntV_i, fracM_CV.at(bin), ms_to_data_pot[(*mc_it)->GetName()],fracM_CV_POT  );
        
        fracM_ii *= (fracM_CV_POT * fracM_CV_POT);  //Get diagonal element of the full error matrix before multisim is scaled to data POT
        fracM_ii -= fracM_CV_POT;                   //subtract the statistical error built in multisims
        if(fracM_ii<0.) fracM_ii = fabs(fracM_ii);	// if error is negative. Then scale it positively. (Added by EC)
        fracM_ii *= ( evntV_i / fracM_CV_POT ) * ( evntV_i / fracM_CV_POT );  //scale the systematic error to MC cv number of events
        fracM_ii += evntV_i;                       //add back the statistical error 
        fracM_ii /= (evntV_i*evntV_i);            //calculate the fractional error

        oss <<Form("%14.6f\n",fracM_ii);
        fFracErrorMatrix[matrix_name]->SetElement(bin,bin,fracM_ii);
        bin++;
       }
     }
     mc_it++;
   }
   MsgInfo(oss.str());
   oss.str("");

if (MsgLog::GetGlobalLogLevel() >= MsgLog::kDebug){
  MsgDebug(2, "Dumping matrix " + matrix_name);
  fFracErrorMatrix[matrix_name]->Dump();
}

}

 void FitMaster::AddFullOscStat()
 {
  MsgDebug(2, " ");
  if ( !fAddSignalStatError ) return;
  if(fReadFromLibrary){
    MsgDebug(2, "fReadFromLibrary");
    return;
  }

  Int_t bin = 0;
  for ( auto const& mc_it : fMCSampleList ) {
    if ( (mc_it->GetName()).find("Signal") == std::string::npos ) {
      bin += mc_it->GetNbins();
    } 
    else {
      for (int i=0; i<mc_it->GetNbins(); i++) {
        Double_t m_ii = fTotalFullErrorMatrix->GetElement(bin,bin);
        m_ii += fEventVector.at(bin);
        fTotalFullErrorMatrix->SetElement(bin, bin, m_ii);
        bin++;
      }
    }
  } // for(mc_it:fMCSampleList)
}

void FitMaster::AddStatError()
{
  //function adds mc number of events to diagonal

  MsgDebug(2, " ");
  Int_t bin = 0;
  for ( auto const& data_it : fDataSampleList ) {
    std::string name=data_it->GetName();
    name.replace(name.find("Data"),4,"MC");
    std::shared_ptr<Sample> mcs=GetMCSample(name);
    for (int i=0; i<data_it->GetNbins(); i++) {
      Double_t m_ii = fCollapsedErrorMatrix->GetElement(bin,bin);
      m_ii += mcs->GetBinContent(i);
      fCollapsedErrorMatrix->SetElement(bin, bin, m_ii);
      bin++;
    }
  }
}
void FitMaster::RemoveFullOscStat()
{
  /*
    This function removes the statistical error of the signal that can be added with
    AddFullOscStat() function
  */
  MsgDebug(2, " ");
    Int_t bin = 0;
    for ( auto const& mc_it : fMCSampleList ) {
      if ( (mc_it->GetName()).find("Signal") == std::string::npos ) {
        bin += mc_it->GetNbins();
      } else {
        for (int i=0; i<mc_it->GetNbins(); i++) {
         Double_t m_ii = fTotalFullErrorMatrix->GetElement(bin,bin);
         m_ii -= fEventVector.at(bin);
         fTotalFullErrorMatrix->SetElement(bin, bin, m_ii);
         bin++;
       }
     }
   }
 }

 void FitMaster::SetParameter(std::string name, Double_t par)
 {
  for ( auto const& mc_it : fMCSampleList ) {
    std::vector<std::shared_ptr<WeightCalc>> wc = mc_it->GetWeightCalc();
    for (unsigned int i=0; i < wc.size(); i++) {
      wc[i]->SetParameter(name, par);
    }
  }
  if(fIncludeLSND){
    fLSNDWeightCalc->SetParameter(name, par);
  }

  // for fake data: set the parameter values and create a library ticket for future references
  unsigned int nPars = fParNames.size();
  if(nPars!=0){
    unsigned int pos = distance(fParNames.begin(), find(fParNames.begin(), fParNames.end(), name));
    if(pos < fParValues.size()){
      fParValues.at(pos) = par;
      fLibraryTicket = "";
      for(double parValue : fParValues)
        fLibraryTicket += MsgLog::Form("_%09.5f", parValue);
    }
    else
      MsgError(MsgLog::Form("Warning: fParValues has size %d while fParNames has size %d", fParValues.size(), fParNames.size()));
  }
}

void FitMaster::SetFakeDataParameter(std::string name, Double_t val)
{
  fFakeDataParameter[name]=val;
}

std::list<std::shared_ptr<Sample> > FitMaster::GetListOfSamples(std::string type)
{
  std::list<std::shared_ptr<Sample> > sample_list;

  for ( auto const& it : fMCSampleList ) {
    if ( it->GetName().find(type) != std::string::npos ) sample_list.push_back(it);
  }
  for ( auto const& it : fDataSampleList ) {
    if ( it->GetName().find(type) != std::string::npos ) sample_list.push_back(it);
  }
  return sample_list;
}

Double_t FitMaster::GetChi2()
{
  MsgDebug(2, " ");
  Double_t chi2=0;
  
  unsigned int nbin = fNdataBins;
  unsigned int i    = nbin;
  do {
    i--;
    unsigned int j=nbin;
    do {
      j--;
      chi2 += ( (fCollapsedEventVector.at(i)-fDataVector.at(i))
               * fInvertedCollapsedErrorMatrix->GetElement(i,j)
               *(fCollapsedEventVector.at(j)-fDataVector.at(j)) );
    } while (j>0);
  } while (i>0);

  chi2+=fLogDeterminant;
  if(fIncludeLSND) chi2 += GetLSNDChi2();
  return chi2;
}
void   FitMaster::SetLSNDWeightCalc(std::shared_ptr<WeightCalc> wc){
    fIncludeLSND        = true;
    fLSNDWeightCalc     = wc;
    MsgInfo("LSND is included in fit with weight calculation " + fLSNDWeightCalc->GetName());
}

Double_t FitMaster::GetLSNDChi2()
{
  static const int nLSND = 8;
  static const double xLSND[nLSND]    = {0.44843, 0.54933, 0.65022, 0.75112, 0.85000, 0.94918, 1.05007, 1.30045};
  static const double yLSNDErr[nLSND] = {0.00127002, 0.00058352, 0.0005492 , 0.0009611 , 0.00205955,
                                         0.00343249, 0.00542334, 0.00758581 };
  static const double yLSND[nLSND]    = {0.00185267467753 , 0.00123205508409 , 0.00068008537624 , 0.00150111338006 , 
                                         0.00293999035413 , 0.00479086925737 , 0.00375835035043 , 0.00464392361289};
  static const double L = 0.030; // LSND distance in km

  double E[nLSND];
  double chi2 = 0.;
  double prob = 0.;
  for(int i = 0; i <nLSND; i++){
    E[i]  = L / xLSND[i];
    prob  = fLSNDWeightCalc->GetWeight(L, E[i], 4);
    chi2 += pow( (yLSND[i]-prob)/yLSNDErr[i], 2);
  }
  return chi2;
}

void FitMaster::Run()
{
  MsgDebug(2, " ");
  ostringstream oss;
  // ============== FAKE ===================
  if (fDoFakeDataFits && fNfakeDataFits>0) {
    for (auto const& fdpar_it : fFakeDataParameter) {
      SetParameter(fdpar_it.first, fdpar_it.second);
    }

    UpdateHistograms();     // update MC histograms
    BuildEventVector();
    BuildFullErrorMatrix();
    AddFullOscStat();       // ?? Add eventvector to the diagonal terms of the error matrix.
    BuildCollapsedEventVector();
    BuildCollapsedErrorMatrix();

    MsgInfo("Generating fake data... ");
    std::vector<std::vector<Double_t> > fake_data;

    //----------------
    //fake + real: No longer active!!!!
    //----------------
    if ( fAddFakeDataToRealData && fDataPOT > 0) {
      MsgInfo(MsgLog::Form( "Generating fake data for %.3f POT and adding to data (%.3f POT)", fTotPOT-fDataPOT, fDataPOT) );
      // need to build matrix for (fTotPOT-fDataPOT), generate fake data and add it to real data
      // first subtract the stat error
      ErrorMatrix fakemat;
      fakemat.CopyFrom(fTotalFullErrorMatrix);
      std::vector<Double_t> fakevec=fEventVector;

      for (Int_t i=0;i<fakemat.GetDimX();i++) {
        Double_t m_ii=fakemat.GetElement(i,i);
        m_ii -= fEventVector.at(i);
        fakemat.SetElement(i,i,m_ii);
      }
      // scale sys error and add stat error
      for (Int_t i=0;i<fakemat.GetDimX();i++) {
        for (Int_t j=0;j<fakemat.GetDimX();j++) {
          Double_t m_ij=fakemat.GetElement(i,j);
          m_ij *= (fTotPOT-fDataPOT)/fTotPOT*(fTotPOT-fDataPOT)/fTotPOT;
          if ( i == j) {
            fakevec[i] *= (fTotPOT-fDataPOT)/fTotPOT;
            m_ij += fakevec[i];
          }
          fakemat.SetElement(i,j,m_ij);
        }
      }
      fake_data=Util::GetFakeData(fFakeDataRandomSeed,fNfakeDataFits,fakevec,fakemat);    
      //This part adds fake data to real data
      for (unsigned int i=0;i<fake_data.size();i++) {
        Int_t k=fDataVector.size();
        for (int j= (int) fake_data.at(i).size()-1;j>=0;j--) {
          k--;
          if (k>=0 ) {
            fake_data.at(i).at(j) += fDataVector.at(k);
          }
        }
      }
    }
    // ---------------------------
    // Fake only, not add to data : move inside the fake data
    // ---------------------------
    else {
      MsgInfo("Checking whether Error Matrix that generates fake data is POS DEF:");
      fTotalFullErrorMatrix->MakePosDef(1);
      fake_data=Util::GetFakeData(fFakeDataRandomSeed,fNfakeDataFits,fEventVector, *fTotalFullErrorMatrix);
      
      // change to use collapsed event vector to generate fake data so that it got lower chance not to be positive definite
      //fCollapsedErrorMatrix->MakePosDef(false); // not dry run here! Needs to be real!
      //fake_data=Util::GetFakeData(fFakeDataRandomSeed,fNfakeDataFits,fCollapsedEventVector,fCollapsedErrorMatrix);       
    }


    MsgInfo("Done!");
    MsgDebug(2,"Fake data: ");
    for (unsigned int i=0;i<fake_data.size();i++) {
      oss <<setw(6)<<i;
      for (unsigned int j=0;j<fake_data.at(i).size();j++) 
        oss <<setw(10)<<setiosflags(ios::fixed)<<setprecision(2)<<fake_data.at(i).at(j);
      oss<<endl;
    }
    MsgDebug(2,oss.str());
    oss.str("");


    //Added for spped, so that the rootfile is open just once
    char* file_name=Form("%s_fake", fOutputFileStem.c_str());
    MsgInfo(MsgLog::Form("stem = %s", file_name));
    for (auto const& fdpar_it : fFakeDataParameter) {
      file_name=Form("%s_%s_%f",file_name, fdpar_it.first.c_str(),fdpar_it.second);
    }
    MsgInfo(MsgLog::Form("createing file %s.root",file_name));

    foutFake =new TFile(Form("%s.root",file_name),"RECREATE");
    treeFake =new TTree("fit","Fit Results");
    foutFake->mkdir("Signal");
    foutFake->mkdir("Data");

    // prepare library to save all the fCollapsedEventVector, fCollapsedErrorMatrix, and the inverted ones

    for (fCurrentFakeDataFitN=0;fCurrentFakeDataFitN<fNfakeDataFits;fCurrentFakeDataFitN++) {
      MsgInfo("###############################################################")
      MsgWarning(MsgLog::Form("Running Fake Data # %d", fCurrentFakeDataFitN));
      MsgInfo("###############################################################")
      //remove data samples if there are any
      fDataSampleList.erase(fDataSampleList.begin(),fDataSampleList.end());

      //Now loop through MC list and create fake data samples in same order
      Int_t firstbin=0;
      for ( auto const& it : fMCSampleList ) {
        Int_t nbins      =it->GetNbins();
        std::string name =it->GetName();

        if (name.find("Signal") != std::string::npos)  // e.g. Nue Signal -> Nue Data
          name.replace(name.find("Signal"),6,"Data");
        else if (name.find("MC") != std::string::npos) // e.g. Nue MC     -> Nue Data
          name.replace(name.find("MC"),2,"Data");
        else
          MsgWarning( "Sample name "+name+"not recognized! Expected Signal or MC");

        bool already_have =false;
        auto dit = fDataSampleList.begin();
        while ( dit!=fDataSampleList.end() && !already_have ) {
          if ( (*dit)->GetName() == name ) {
            already_have = true;
          }
          else dit++;
        }
        if ( already_have ) {
          std::vector<Double_t> data;
          data.resize(nbins);	

          for (Int_t i=0;i<nbins;i++) {
            data.at(i)=fake_data.at(fCurrentFakeDataFitN).at(firstbin+i) + (*dit)->GetBinContent(i);
          }
          (*dit)->FillHistogram(data);	
        } else {
          std::vector<Double_t> data;
          data.resize(nbins);	
          std::shared_ptr<Sample> data_sample = std::make_shared<Sample>();
          data_sample->SetName(name);
          data_sample->SetBins(it->GetBins());
          for (Int_t i=0;i<nbins;i++) data.at(i)=fake_data.at(fCurrentFakeDataFitN).at(firstbin+i);
            data_sample->FillHistogram(data);
          AddDataSample(data_sample);
        }
        firstbin+=nbins;
      } // for(fMCSampleList)

      BuildDataVector();
      //Dump();
      MsgLog::Flush();
      for ( auto const& algo_it : fRunAlgo ) {
        fParNames = algo_it.second->GetParNames();
        fParValues.clear();
        fParValues.resize(fParNames.size());
        (algo_it.second)->Run();
      }
      MsgLog::Flush();
      fReadFromLibrary = true; // no longer generate new matrices!!!! 

      if(fCurrentFakeDataFitN!=0 && fCurrentFakeDataFitN%100==0){
         treeFake->Write("", TObject::kOverwrite);
         foutFake->Flush();
      }


    } // end of fCurrentFakeDataFitN
    treeFake->Write();
    foutFake->Close();
    delete foutFake;
    foutFake=0;
    treeFake=0;

  }

  // ============ NOT FAKE =================
  else {
    for ( auto const& algo_it : fRunAlgo ) {
      (algo_it.second)->Run();
    }
  }
}


void FitMaster::CalculateErrors(std::vector<std::string> parname, std::vector<Double_t> par, std::string name, TFile* fout) 
{

  //if file pointer is provided routine dumps histograms with 
  //unconstrained errors
  //constrained number of events, constrained errors 
  //and constrained matrix
  StartWatch("FitMaster::CalculateErrors");
  bool tempReadFromLibrary = fReadFromLibrary;
  fReadFromLibrary = false;
  MsgInfo("*********************************************************************************");
  MsgInfo("* Errors calculated for:                                                        *");
  stringstream oss;

  for (unsigned int i=0; i < parname.size(); i++) {
    MsgInfo(MsgLog::Form("* %10s%23.4f%50s", parname.at(i).c_str(), par.at(i), "*"));
    SetParameter(parname.at(i), par.at(i));
  }
  MsgInfo("*********************************************************************************");
  UpdateHistograms();
  BuildEventVector();
  BuildFullErrorMatrix();
  AddFullOscStat();
  BuildCollapsedErrorMatrix();

  const ErrorMatrix matrix  = GetCollapsedErrorMatrix();

  std::list<std::vector<Double_t> > unconstrained_errors;
  std::list<std::vector<Double_t> > unconstrained_events;
  unsigned int bin=0;
  unsigned int bin_off =0;
  if(fNfakeDataFits!=0){
    MsgInfo( "-----------------------------");
    MsgInfo( "Unconstrained:");
    MsgInfo( "-----------------------------");
  }
  for ( auto const& it : fMCSampleList ) {
    if ( it->GetName().find("Signal") == std::string::npos && it->GetNbins()>0 ) { // not signal(that's background) and has bin(s)
      std::vector<Double_t> sample_unconstrained_errors;
      std::vector<Double_t> sample_unconstrained_events;
      Double_t total_error =0.;

      for (int i = 0; i<it->GetNbins(); i++) {
       Double_t ev=it->GetBinContent(i);

        // find corresponding signal (excess). If found, add it.
        if ( fBackgroundSignal.find(it->GetName()) != fBackgroundSignal.end() )
          ev += GetMCSample(fBackgroundSignal[it->GetName()])->GetBinContent(i);
        sample_unconstrained_events.push_back(ev);

        // unconstrained error is the sqrt(Mii), where Mii is the corresponding diagonal term of final matrix
        sample_unconstrained_errors.push_back(sqrt(matrix.GetElement(bin,bin)));
        bin++;

        // total error: Sum(i= binoff-> binoff+n) Sum(j=binoff -> binoff+n) Mij (upper half of matrix element in the small block)
        for (int j = 0; j<it->GetNbins(); j++) {
          total_error += matrix.GetElement(i+bin_off,j+bin_off);
        } // for(j)
      } // for(i)
     total_error = sqrt(total_error);
     bin_off = bin;

     oss<<"Sample name : "<<it->GetName()<<endl;
     oss<<"event vector: ";
     for (unsigned i=0;i<sample_unconstrained_events.size();i++) 
       oss<<setw(10)<<setprecision(6)<<sample_unconstrained_events.at(i);
     oss<<endl;
     oss<<"event error:  ";
     for (unsigned  i=0;i<sample_unconstrained_events.size();i++) 
       oss<<setw(10)<<setprecision(6)<<sample_unconstrained_errors.at(i);
     oss<<endl;
     oss<<endl;
     it->SetErrors(sample_unconstrained_errors);
     oss<<"Total error (sqrt(sum Mij)): "<<total_error<<endl;
     oss <<endl;
     if(fNfakeDataFits!=0) MsgInfo(oss.str());
     oss.str("");
   }
  } // for(it: fMCSampleList)

  // Constraining. Please see technote 221 and A.A. Aguilar-Arevalo's thesis
  // M = fCollapsedErrorMatrix
  // IM = M^(-1)
  // for constraining bin i
  // IM'ii = IMii + 1. / Ni * delta_{i==j, constraining bin}
  // for example, if we are using numu to constrain, then the diagonal term we added 1/N_i(numu)
  

  if(fNfakeDataFits!=0){
    MsgInfo( "-----------------------------");
    MsgInfo("Constrained (using data):");
    MsgInfo( "-----------------------------");
  }
  std::list<std::vector<Double_t> > constrained_errors;
  std::list<std::vector<Double_t> > constrained_events;
  ErrorMatrix constrained_matrix;                         // used to store constrained matrix
  ErrorMatrix collapsed_matrix;                           // will store unconstrained matrix(collapsedErrorMatrix)
  constrained_matrix.CopyFrom(fCollapsedErrorMatrix);
  collapsed_matrix.CopyFrom(fCollapsedErrorMatrix);
  constrained_matrix.SetName(Form("collapsed_error_matrix_constrained_%s",name.c_str()));
  constrained_matrix.Invert();


  // loop through MCSamples(ignoring the Signals), then find the corresponding data
  // If the bin of the data is to be used as constraining one, the add 1/Ni to the constrained M
  // otherwise, count it as constrained bin data
  bin_off =0;
  bin = 0;
  std::vector<Int_t>    constrained_bin(constrained_matrix.GetDimX());  // whether bin is constrained
  std::vector<Double_t> constrained_bin_mc;                             // n events for constrained MC
  std::vector<Double_t> constrained_bin_data;                           // n events for constrained bins
 for ( auto const& MCSample : fMCSampleList ) {
  if ( MCSample->GetName().find("Signal") == std::string::npos && MCSample->GetNbins()>0 ) { // Nue MC, Numu MC, Nuebar MC, Numubar MC
      // && fBackgroundSignal.find(MCSample->GetName()) == fBackgroundSignal.end()) {

    std::string name             = MCSample->GetName();
    std::vector<Int_t> rebin_vec = fRebinMap[name];
    std::vector<Double_t> data_events;  

    // find the corresponding data set
    name.replace(name.find("MC"),2,"Data");
    auto DataSample=fDataSampleList.begin();         
    while ( name.compare((*DataSample)->GetName()) != 0) DataSample++;

    // if bin value is negative, then use it for constrain
    for (int i = 0; i<MCSample->GetNbins(); i++) {
      bool usebin=false;
      for (unsigned ibin=0;ibin<rebin_vec.size();ibin++) {
        if (abs(rebin_vec.at(ibin))==i+1 && rebin_vec.at(ibin)<0)  usebin=true;
      }

      data_events.push_back((*DataSample)->GetBinContent(i));
      constrained_bin[bin+bin_off]=1;	
      if (data_events.at(i) != 0 && usebin) {     // if usebin, then this is a constraining bin, not constrained
        constrained_matrix.SetElement(bin+bin_off, bin+bin_off, 
                                      constrained_matrix.GetElement(bin+bin_off,bin+bin_off) + 1./data_events.at(i) );
        constrained_bin[bin+bin_off]=0;
      } else {
        if (data_events.at(i)==0) MsgInfo(MsgLog::Form("Bin %d is zero", i ));
      }
      if (!usebin) {                              // if not usebin, then it is a constrained bin
        constrained_bin_data.push_back(data_events.at(i));
      }
      bin++;
    }
    bin_off += MCSample->GetNbins();
    bin=0;
  }
 }

constrained_matrix.Invert();

//write to file if file is open
if ( fout!=0 ) {
  TMatrixD matrixConstrained(constrained_matrix.GetDimX(),constrained_matrix.GetDimY());
  TMatrixD matrixUnconstrained(constrained_matrix.GetDimX(),constrained_matrix.GetDimY());

  for (Int_t i=0;i<constrained_matrix.GetDimX();i++) {
    for (Int_t j=0;j<constrained_matrix.GetDimY();j++) {
     matrixConstrained(i,j)            = constrained_matrix.GetElement(i,j);
     matrixUnconstrained(i,j)          = collapsed_matrix.GetElement(i,j);
   }
 }
 matrixConstrained.Write(Form("collapsed_error_matrix_constrained_%s",name.c_str()));
 matrixUnconstrained.Write(Form("collapsed_error_matrix_unconstrained_%s",name.c_str()));

}
collapsed_matrix.Invert();


// complicated vector:
// constrained MC Nfit_i is equal to 
// Nfit_i = B * (M^-1 * N_MC + delta(numu bins) )
// where B is the constrained matrix, M, collapsed matrix
// See A.A. Aguilar-Arevalo's thesis p.146
std::vector<Double_t> complicated_vector(fCollapsedEventVector.size());
Int_t bin_i=0;
Int_t bin_j=0;
bin_off=0;

for ( auto const& MCSample1 : fMCSampleList ) {
  if ( MCSample1->GetName().find("Signal") == std::string::npos && MCSample1->GetNbins()>0 ) {
    std::vector<Int_t> rebin_vec=fRebinMap[MCSample1->GetName()];
    for (Int_t k=0; k<MCSample1->GetNbins(); k++) {
      complicated_vector[bin_i+k] = 0.;
      for ( auto const& MCSample2 : fMCSampleList ) {
        if ( MCSample2->GetName().find("Signal") == std::string::npos && MCSample2->GetNbins()>0 ) {
          for (Int_t j=0;j<MCSample2->GetNbins();j++) {
            Double_t ev=MCSample2->GetBinContent(j);
            if ( fBackgroundSignal.find(MCSample2->GetName()) != fBackgroundSignal.end() )
              ev += GetMCSample(fBackgroundSignal[MCSample2->GetName()])->GetBinContent(j);
            complicated_vector[bin_off+k] += collapsed_matrix.GetElement(bin_i+k,bin_j+j)*ev;
          }
          bin_j += MCSample2->GetNbins();
        }
      } //for(MCSample2:fMCSampleList)
      bin_j = 0;
      bool usebin=false;
      for (unsigned int bin=0;bin<rebin_vec.size();bin++) 
        if (abs(rebin_vec[bin])==k+1 && rebin_vec[bin]<0) usebin=true;

      if ( usebin ) complicated_vector[bin_off+k] += 1.;
    } // for(k in MCSample1 bins)
    bin_off += MCSample1->GetNbins();
    bin_i += MCSample1->GetNbins();
  }
} //for(MCSample1:fMCSampleList)

//now loop one more time to get corrected vector, Nfit_i
std::vector<Double_t> corr_vector(fCollapsedEventVector.size());
bin_i = 0;
bin_j = 0;
for ( auto const& MCSample1 : fMCSampleList ) {
  if ( MCSample1->GetName().find("Signal") == std::string::npos && MCSample1->GetNbins()>0 ) {
    for (Int_t k=0; k<MCSample1->GetNbins(); k++) {
      corr_vector[bin_i+k] = 0.;
      for ( auto const& MCSample2 : fMCSampleList ) {
        if ( MCSample2->GetName().find("Signal") == std::string::npos && MCSample2->GetNbins()>0 ) {
          for (Int_t j=0;j<MCSample2->GetNbins();j++) {
            corr_vector[bin_i+k] += constrained_matrix.GetElement(bin_i+k,bin_j+j) * complicated_vector[bin_j+j];
          }
          bin_j += MCSample2->GetNbins();
        }
      } //for(MCSample2:fMCSampleList)
      bin_j = 0;
    }

    // save the corrected vector and its error to file
    std::vector<Double_t> tmp_data; // temporary vector for corr_vector
    std::vector<Double_t> tmp_err;  // sqrt(constrained_matrix_ii)
    for (Int_t i=bin_i;i<MCSample1->GetNbins()+bin_i;i++) {
      std::vector<Int_t> rebin_vec=fRebinMap[MCSample1->GetName()];
      bool usebin=false;
      for (unsigned int ibin=0;ibin<rebin_vec.size();ibin++) {
        if (abs(rebin_vec[ibin])==i+1-bin_i && rebin_vec[ibin]<0)  usebin=true;
      }
      if (!usebin) constrained_bin_mc.push_back(corr_vector.at(i));

      tmp_data.push_back(corr_vector.at(i));
      tmp_err.push_back(sqrt(constrained_matrix.GetElement(i,i)));
    }

    std::shared_ptr<Sample> tmp_sample = std::make_shared<Sample>();
    tmp_sample->SetName(Form("%s constrained %s",MCSample1->GetName().c_str(),name.c_str()));
    tmp_sample->SetBins(MCSample1->GetBins());
    tmp_sample->FillHistogram(tmp_data);
    tmp_sample->SetErrors(tmp_err);
    if (fout)
      tmp_sample->WriteHistogramsToFile(fout);


   oss <<"Sample name: "<<tmp_sample->GetName()<<endl;
   oss<<"event vector: ";
   for (unsigned int i=0;i<tmp_data.size();i++) 
     oss<<setw(10)<<setprecision(6)<<tmp_data.at(i);
   oss<<endl;
   oss<<"event error:  ";
   for (unsigned int i=0;i<tmp_err.size();i++) 
     oss<<setw(10)<<setprecision(6)<<tmp_err.at(i);
   oss<<endl;
   if(fNfakeDataFits!=0) MsgInfo(oss.str());
   oss.str("");

   bin_i += MCSample1->GetNbins();
 }
} //for(MCSample1:fMCSampleList)

if(MsgLog::GetGlobalLogLevel() >= MsgLog::kDebug){

  MsgInfo( "-----------------------------");
  MsgInfo( "Constrained collapsed matrix:");
  MsgInfo( "-----------------------------");
  constrained_matrix.Dump();


  MsgInfo( "-----------------------------");
  MsgInfo( "fCollapsedErrorMatrix:");
  MsgInfo( "-----------------------------");
  fCollapsedErrorMatrix->Dump();


  MsgInfo( "-----------------------------");
  MsgInfo( "fTotalFullErrorMatrix:");
  MsgInfo( "-----------------------------");
  fTotalFullErrorMatrix->Dump();


  MsgInfo( "-----------------------------");
  MsgInfo( "fTotalFracErrorMatrix:");
  MsgInfo( "-----------------------------");
  fTotalFracErrorMatrix->Dump();
}
  //now calculate constrained chi2
Int_t n_constrained_bins=0;
for (unsigned int i=0;i<constrained_bin.size();i++) {
  if (constrained_bin.at(i)>0) n_constrained_bins++; 
}
ErrorMatrix temp_constr_mat; 
temp_constr_mat.SetName("constrained part of the matrix");
temp_constr_mat.SetVerbosity(fVerbosity);
temp_constr_mat.Resize(n_constrained_bins,n_constrained_bins);
bin_i=0;
bin_j=0;
for (unsigned int i=0;i<constrained_bin.size();i++) {
  if (constrained_bin.at(i)) {
    for (unsigned int j=0;j<constrained_bin.size();j++) {
     if (constrained_bin[j]) {
       temp_constr_mat.SetElement(bin_i,bin_j,constrained_matrix.GetElement(i,j));
       bin_j++;
     }
   }
   bin_j=0;
   bin_i++;
 }
}
Double_t logmat=log(temp_constr_mat.GetDeterminant());
if(fNfakeDataFits==0)
  temp_constr_mat.Dump();

temp_constr_mat.Invert();

Double_t nue_chi2=0;
oss <<endl;
oss << "-----------------------------"<<endl;
oss << "Constrained bins mask:"<<endl;
for (unsigned int i=0;i<constrained_bin.size();i++) {
  oss<<setw(9) << setprecision(6)<<constrained_bin.at(i);
  if((i+1)%15==0) oss << endl;
}
oss<<endl;

oss << "Constrained bin data:"<<endl;
for (unsigned int i=0;i<constrained_bin_data.size();i++) {
  oss<<setw(9) << setprecision(6)<<constrained_bin_data.at(i);
  if((i+1)%15==0) oss << endl;
}
oss<<endl;

oss << "Constrained bin MC:"<<endl;
for (unsigned int i=0;i<constrained_bin_mc.size();i++) {
  oss<<setw(9) << setprecision(6)<<constrained_bin_mc.at(i);
  if((i+1)%15==0) oss << endl;
}
oss<<endl;
oss << "-----------------------------"<<endl;
if(fNfakeDataFits!=0) MsgInfo(oss.str());
oss.str("");

for (unsigned int i=0;i<constrained_bin_mc.size();i++) {
  for (unsigned int j=0;j<constrained_bin_mc.size();j++) {
    nue_chi2+=(constrained_bin_data.at(i)-constrained_bin_mc.at(i))*
    temp_constr_mat.GetElement(i,j)*
    (constrained_bin_data[j]-constrained_bin_mc[j]);
  }
}

if(fIncludeLSND) nue_chi2 += GetLSNDChi2();

MsgInfo( "------------------------------------------");
MsgInfo(MsgLog::Form("Constrained CHI2 %s       = %10.5f", name.c_str(),nue_chi2));
MsgInfo(MsgLog::Form("Constrained LIKELIHOOD %s = %10.5f", name.c_str(),nue_chi2+logmat));
MsgInfo( "------------------------------------------");

if(fNfakeDataFits>0){
  MsgError(MsgLog::Form("Constrained CHI2 %s       = %10.5f", name.c_str(),nue_chi2));
  MsgError(MsgLog::Form("Constrained LIKELIHOOD %s = %10.5f", name.c_str(),nue_chi2+logmat));
}
MsgLog::Flush();
fReadFromLibrary = tempReadFromLibrary;

StopWatch("FitMaster::CalculateErrors");


} // FitMaster::CalculateErrors()

void FitMaster::SetVerbosity(Int_t val)
{
  fVerbosity = val;

  for(auto& MCSample   : fMCSampleList  ) MCSample  ->SetVerbosity(fVerbosity);
  for(auto& DataSample : fDataSampleList) DataSample->SetVerbosity(fVerbosity);

  //for_each(fMCSampleList.begin(),   fMCSampleList.end(),   bind2nd(mem_fun(&Sample::SetVerbosity), fVerbosity));
  //for_each(fDataSampleList.begin(), fDataSampleList.end(), bind2nd(mem_fun(&Sample::SetVerbosity), fVerbosity));

  for ( auto& em_it : fFracErrorMatrix ) {
    em_it.second->SetVerbosity(fVerbosity);
  }
  
  fTotalFracErrorMatrix->SetVerbosity(fVerbosity);
  fTotalFullErrorMatrix->SetVerbosity(fVerbosity);
  fCollapsedErrorMatrix->SetVerbosity(fVerbosity);
  fInvertedCollapsedErrorMatrix->SetVerbosity(fVerbosity);
}

void FitMaster::SetOutputFileStem(std::string s) {
  fOutputFileStem = s; 
}

void FitMaster::Dump()
{ // Dump if it's MsgLog level 3(kInfo) or above
  if (fVerbosity <= 2 ) return;
  ostringstream oss;
  MsgInfo("******************************************************************");
  MsgInfo("MC samples (fMCSampleList) included in the fit: ");
  for(auto it=fMCSampleList.begin(); it!=fMCSampleList.end(); it++) oss << (*it)->GetName() << ", ";
  MsgInfo(oss.str());
  oss.str("");
  for(auto const& MCSample : fMCSampleList) MCSample->Dump();

  MsgInfo("******************************************************************");
  MsgInfo("Data samples (fDataSampleList) included in the fit: " );
  for(auto it=fDataSampleList.begin(); it!=fDataSampleList.end(); it++) oss << (*it)->GetName() << ", ";
  MsgInfo(oss.str());
  oss.str("");
  for(auto const& DataSample : fDataSampleList) DataSample->Dump();

  MsgInfo(MsgLog::Form("%20s%20s  (fBackgroundSignal pairs)", "Background sample", "Signal sample") );
  for (Int_t i=0;i<40;i++) oss << "-";
  oss<<endl;
  for ( auto const& sb_it : fBackgroundSignal ) { // background Signal pair map
    oss << setw(20) << sb_it.first << setw(20) << sb_it.second<< endl;
  }
  MsgInfo(oss.str());
  oss.str("");


  MsgInfo("\n******************************************************************");
  MsgInfo("Matrices included in the fit: " );
  for ( auto const& em_it : fFracErrorMatrix ) {
    MsgInfo(em_it.second->GetName());
  }
  MsgInfo("******************************************************************");
  MsgInfo("Running algorithms: " );
  for ( auto const& algo_it : fRunAlgo ) {
    algo_it.second->Dump();
  }
  
  if ( !isTotalRecent ) 
    MsgWarning("Warning::Total Matrix has not been calculated with all sub matrices");
  
  //if ( fVerbosity < 4 ) return;
  
  oss << " Bin Vector: "<<endl;
  for (unsigned int i=0; i< fMCBins.size(); i++) {oss <<setw(9) << std::setprecision(6)<<fMCBins.at(i);}
  oss <<endl;

  oss << " Event Vector: "<<endl;
  for (unsigned int i=0; i< fEventVector.size(); i++) {oss <<setw(9) << std::setprecision(6)<<fEventVector.at(i);}
  oss <<endl;

  oss << " Collapsed Event Vector: "<<endl;
  for (unsigned int i=0; i< fCollapsedEventVector.size(); i++) {oss <<setw(9) << std::setprecision(6)<<fCollapsedEventVector.at(i);}
  oss <<endl;

  oss << " Data Vector: "<<endl;
  for (unsigned int i=0; i< fDataVector.size(); i++) {oss <<setw(9) << std::setprecision(6)<<fDataVector.at(i);}
  oss <<endl;
  MsgInfo(oss.str());
  oss.str("");
  if ( MsgLog::GetGlobalLogLevel() < MsgLog::kDebug ) return;
  MsgInfo( "Print Error Matrices");
  fTotalFracErrorMatrix->Dump();
  fTotalFullErrorMatrix->Dump();
  fCollapsedErrorMatrix->Dump();

  if ( MsgLog::GetGlobalLogLevel() < MsgLog::kDebug ) return;

  MsgInfo( "******************************************************************");
  MsgInfo( "Matrices included in the fit: " );
  for ( auto const& em_it : fFracErrorMatrix ) {
    em_it.second->Dump();
  }

}


std::vector<std::string>  FitMaster::GetListOfFracErrorMatrixNames(){
  std::vector<std::string> matrixNames;

  for ( auto const& it : fFracErrorMatrix) {
    if(it.second==NULL){
      MsgError("FracErrorMatrix is null!");
    }
    matrixNames.push_back(it.first); // get name
  }
  return matrixNames;


}
