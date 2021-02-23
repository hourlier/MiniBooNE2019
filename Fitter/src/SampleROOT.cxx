#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <memory>

#include "Event.h"
#include "Consts.h"
#include "SampleROOT.h"
#include "MsgLog.h"

#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TRandom3.h"
#include "TDirectory.h"
#include "TSystem.h"
#include "FuncStopwatch.h"





using namespace std;

Sample::Sample()
{
  fBins.clear();
  fData.clear();
  fHistContent.clear();
  fHistError.clear();
  fHistError.resize(0);
  fHistSumw2.clear();
  fDataFromNtuple = false;
  fTolerance = 0.0001;
  fWC.clear();
  fNevents=0;
  fNeedUpdate = true;
  fFillBkgHists = true;
  fDownsample = 1;
}

Sample::~Sample()
{
}

std::shared_ptr<Sample> Sample::Clone()
{
  std::shared_ptr<Sample> new_sample = std::make_shared<Sample>();
  new_sample->SetName(fName);
  new_sample->SetInputNtuple(fInputFileName);
  new_sample->SetNtupleID(fNtupleID);
  new_sample->SetVerbosity(fVerbosity);
  new_sample->SetBins(fBins);
  for (unsigned int i=0; i<fWC.size(); i++)     
    new_sample->AddWeightCalc(fWC.at(i)->Clone());
  for (unsigned int i=0; i<fFilter.size(); i++) 
    new_sample->AddFilter(fFilter[i]);

  new_sample->SetDownsample(fDownsample);

  new_sample->FillVector();
  new_sample->FillHistogram();

  return new_sample;
}

void Sample::AddWeightCalc(std::shared_ptr<WeightCalc> wc)
{ 
  fWC.push_back(wc); 
  fNeedUpdate = true;
  std::map<std::string, Double_t > parmap = wc->GetParMap();
  fWeightParMap.insert(parmap.begin(), parmap.end() );
}

void Sample::FillVector()
{

  MsgDebug(2, " ");
  TFile* fIn = new TFile(fInputFileName.c_str());
  if (fIn->IsOpen()==false) {
    MsgError( "ERROR: Error opening input file ... (" + fInputFileName + ")" );
    exit(1);
  }
  TTree* T = (TTree*) fIn->Get(Form("h%d",fNtupleID)); // get the tree
  if(T==NULL){
    MsgError(MsgLog::Form( "ERROR: Cannot find tree h%d in %s", fNtupleID, fInputFileName.c_str() ) );
    fIn->ls();
    exit(1);
  }

  Float_t iflux, ibkgd, nuchan, inno, enugen, energy, nuleng, parid, wgt, ispi0, isdirt;
  T->SetBranchAddress("iflux"   , &iflux);
  T->SetBranchAddress("ibkgd"   , &ibkgd);
  T->SetBranchAddress("nuchan"  , &nuchan);
  T->SetBranchAddress("inno"    , &inno);
  T->SetBranchAddress("enugen"  , &enugen);
  T->SetBranchAddress("energy"  , &energy);
  T->SetBranchAddress("nuleng"  , &nuleng);
  T->SetBranchAddress("parid"   , &parid);
  T->SetBranchAddress("wgt"     , &wgt);
  T->SetBranchAddress("ispi0"   , &ispi0);
  T->SetBranchAddress("isdirt"  , &isdirt);
  Int_t nevents = T->GetEntries();

  fData.clear();

  Int_t nskip=0;
  for ( unsigned int ifilt=0; ifilt < fFilter.size(); ifilt++ ) 
    fFilter[ifilt]->Reset();

  TRandom3 rnd;
  rnd.SetSeed(1);
  Long64_t nentries = T->GetEntries();
  for (Long64_t i=0;i<nentries;i++) {
    T->GetEntry(i);
    Event mb;
    
    mb.iflux   ( round(iflux)     );  // QQ: What's iflux?
    mb.ibkgd   ( round(ibkgd)     );  // QQ: The same as the background?
    mb.nuchan  ( round(nuchan)    );
    mb.inno    ( round(inno)      );
    mb.enugen  ( Double_t(enugen) );
    mb.energy  ( Double_t(energy) );
    mb.nuleng  ( Double_t(nuleng) );
    mb.parid   ( round(parid)     );
    mb.wgt     ( Double_t(wgt)    );
    mb.ispi0   ( round(ispi0)     );
    mb.isdirt  ( round(isdirt)    );

    bool keep = true;
    for ( unsigned int ifilt=0; ifilt < fFilter.size(); ifilt++ ) {
      if ( fFilter[ifilt]->Cut(mb) ) keep = false;
    }
    if (fDownsample<1 && fDownsample>0) {
      if (rnd.Uniform()>fDownsample) keep=false; 
       mb.wgt(mb.wgt()/fDownsample); //boost weight of kept events
    }
    if ( keep ) fData.push_back(mb);
    else nskip++;
  
  }

  fIn->Close();
  fNevents = fData.size();

  if ( nevents != nskip+fNevents ) {
    MsgWarning(MsgLog::Form("Warning:: %d found in %s but only %d read!",nevents, fInputFileName.c_str(), fNevents+nskip) );
  }
}

void Sample::FillHistogram(bool useweights, bool fast_hist)
{
  StartWatch("Sample::FillHistogram " + fName);


  //Function fills the histogram using the data input from the ntuple 
  MsgDebug(2, MsgLog::Form("fBins.size()=%d", fBins.size()));
  if (fBins.size() <= 0 ) {
    fHistContent.clear();
    fHistContent.resize(0);
    fHistSumw2.clear();
    fHistSumw2.resize(0);
    MsgWarning(MsgLog::Form("fBins.size() = %d <=0", fBins.size()));
    return;
  }
  if (fBins.size()==1) MsgWarning("fBins.size()==1! It shouldn't be");

  if(fHistContent.size()!=fBins.size()-1) fHistContent.resize(fBins.size()-1);
  if(fHistSumw2.size() != fBins.size()-1) fHistSumw2.resize(fBins.size()-1);
  fill(fHistContent.begin(), fHistContent.end(), 0);
  fill(fHistSumw2.begin()  , fHistSumw2.end()  , 0);

  fUnderflow = 0;
  fOverflow = 0;
  fWeightParMap.clear();
  std::string wghlist="";
  for (unsigned int i=0;i<fWC.size();i++){
    std::map<std::string, Double_t > parmap = fWC.at(i)->GetParMap();
    fWeightParMap.insert(parmap.begin(), parmap.end() );
    if (fast_hist) {
      std::map<std::string, Double_t >::const_iterator pmit=parmap.begin();
      while (pmit!=parmap.end()) {
        std::string tmpstr(Form("%s%9.6e",(pmit->first).c_str(),pmit->second));
        wghlist+=tmpstr;
        pmit++;
      }
    }
  }

  //loop over events and fill the histogram

  if (fast_hist ) { 
    std::map<std::string, std::vector<Double_t> >::const_iterator fhit=fFastHist.find(wghlist);
    if (fhit!=fFastHist.end()) {
      fHistContent=fFastHist[wghlist];
      fNeedUpdate = false;
      return;
    }
  }
  for (unsigned int i=0;i<fData.size();i++) {
    Event   mb      = fData[i];
    Double_t energy = mb.energy();
    Int_t   bin     = FindBin(energy);

    Double_t weight_calc = 1.;
    if (useweights && fWC.size()) {
      for (unsigned int i=0;i<fWC.size();i++) { 
        weight_calc *= fWC.at(i)->GetWeight(mb);
      }
    }

    if      ( bin == Consts::kUnderflow ) fUnderflow += mb.wgt()*weight_calc;
    else if ( bin == Consts::kOverflow  ) fOverflow  += mb.wgt()*weight_calc;
    else {
      fHistContent[bin] += mb.wgt() * weight_calc;
      fHistSumw2[bin]   += mb.wgt() * weight_calc * mb.wgt() * weight_calc;
    }
  }

  fDataFromNtuple = true;
  fNeedUpdate = false;


  if (fast_hist ) fFastHist[wghlist]=fHistContent;

  if (fFillBkgHists) FillBkgHists();
  StopWatch("Sample::FillHistogram " + fName);

}

void Sample::FillBkgHists(bool useweights)
{
//cout << "Fill BkgHists 1" << endl;
  MsgDebug(2, MsgLog::Form("fBins.size=%d", fBins.size()));
  //Function fills the histogram using the data input from the ntuple 
  if (fBins.size() == 0 ) {
    return;
  }
  StartWatch("Sample::FillBkgHists " + fName);

  fHistParent.clear();
  std::vector<Double_t> vec(fBins.size()-1);

  fHistParent.insert(std::make_pair(Consts::kPiPlus,  std::vector<Double_t>(fBins.size()-1)));
  fHistParent.insert(std::make_pair(Consts::kPiMinus, std::vector<Double_t>(fBins.size()-1)));
  fHistParent.insert(std::make_pair(Consts::kKPlus,   std::vector<Double_t>(fBins.size()-1)));
  fHistParent.insert(std::make_pair(Consts::kKMinus,  std::vector<Double_t>(fBins.size()-1)));
  fHistParent.insert(std::make_pair(Consts::kK0L,     std::vector<Double_t>(fBins.size()-1)));
  fHistParent.insert(std::make_pair(Consts::kMuPlus,  std::vector<Double_t>(fBins.size()-1)));
  fHistParent.insert(std::make_pair(Consts::kMuMinus, std::vector<Double_t>(fBins.size()-1)));
  fHistParent.insert(std::make_pair(Consts::kUnknown, std::vector<Double_t>(fBins.size()-1)));
//cout << "Fill BkgHists 2" << endl;

  fHistBackground.clear();
  for ( Int_t i = Consts::BackgroundIter(Consts::Background::kFirst);i<=Consts::BackgroundIter(Consts::Background::kLast); i++ ) {
    std::vector<Double_t> bvec;
    bvec.resize(fBins.size()-1);
    fHistBackground.insert(std::make_pair(Consts::BackgroundIterToEnum(i), bvec));
    //    if ( IsBackground(e, Consts::BackgroundIterToEnum(i)) ) return Consts::BackgroundIterToEnum(i);
  }
//cout << "Fill BkgHists 3" << endl;

  /*
  fWeightParMap.clear();
  std::string wghlist="";
  for (unsigned int i=0;i<fWC.size();i++){
    std::map<std::string, Double_t > parmap = fWC.at(i)->GetParMap();
    fWeightParMap.insert(parmap.begin(), parmap.end() );
  }
  */
  //loop over events and fill the histogram
  for (unsigned int i=0;i<fData.size();i++) {
    Event   mb      = fData[i];
    Double_t energy = mb.energy();
    Int_t   bin     = FindBin(energy);

    Double_t weight_calc = 1.;
    if (useweights && fWC.size()) {
      for (unsigned int i=0;i<fWC.size();i++) { 
       weight_calc *= fWC.at(i)->GetWeight(mb);
      }
    }

    if ( bin != Consts::kUnderflow && bin != Consts::kOverflow) {
      map<Consts::ParticleCode, std::vector<Double_t>  >::iterator pit = fHistParent.find(Consts::ParticleIDtoEnum(mb.parid()));
      if ( pit != fHistParent.end() ) {
        fHistParent[Consts::ParticleIDtoEnum(mb.parid())].at(bin) += mb.wgt()*weight_calc;
      } 
      else 
        MsgWarning(MsgLog::Form("No histogram available for parent type %d", mb.parid()));

      map<Consts::Background::Type, std::vector<Double_t>  >::iterator pit2 = fHistBackground.find(WhichBackground(mb));
      if ( pit2 != fHistBackground.end() ) {
        fHistBackground[WhichBackground(mb)].at(bin) += mb.wgt()*weight_calc;
      } 
      else 
        MsgWarning(MsgLog::Form("No histogram available for background %s", Consts::GetBackgroundName(WhichBackground(mb)).c_str()));
    }  
  }
//cout << "Fill BkgHists 4" << endl;

  StopWatch("Sample::FillBkgHists " + fName);
//cout << "Fill BkgHists 5" << endl;


}

void Sample::FillHistogram(std::vector<Double_t> data)
{
  //function fills the histogram using provided vector
  //the binning has to be provided prior to calling this function using SetBins()
 
  //Function fills the histogram using the data input from the ntuple 
  MsgDebug(2, " ");
  if (fBins.size() <= 0 ) {
    fHistContent.clear();
    fHistContent.resize(0);
    fHistSumw2.clear();
    fHistSumw2.resize(0);
    return;
  }

  if (data.size() != fBins.size()-1) {
    MsgWarning( "Warning::Error while filling histogram for " + fName);
    MsgWarning( MsgLog::Form("Data vector size is %d while number of bins is %d", data.size(), fBins.size()-1));
  }

  fHistContent.clear();
  fHistContent.resize(fBins.size()-1);
  fHistSumw2.clear();
  fHistSumw2.resize(fBins.size()-1);

  for (unsigned int i = 0; i<data.size();i++) {
    fHistContent.at(i) = data.at(i);
    fHistSumw2.at(i)   = data.at(i);
  }
  fDataFromNtuple = false;
  fNeedUpdate = false;
}

void Sample::SetErrors(std::vector<Double_t> err)
{
  if ( err.size() != fHistContent.size() ) {
    MsgWarning(MsgLog::Form("Size mismatch between error vector (%d) and data vector (%d)", err.size(), fHistContent.size()));
    MsgError("Errors for sample " + fName + " not set correctly!");
    return;
  }

  fHistError=err;
}

void Sample::Scale(Double_t scale_factor)
{
  unsigned nbin = fHistContent.size();
  for (unsigned int i=0; i < nbin; i++) {

    fHistContent.at(i) *= scale_factor;
    fHistSumw2.at(i)   *= scale_factor;
  }
  std::map<Consts::ParticleCode, std::vector<Double_t> >::iterator it = fHistParent.begin();
  while ( it != fHistParent.end() ) {
    for (unsigned int i=0; i < nbin; i++) (it->second).at(i) *= scale_factor;
      it++;
  }
  std::map<Consts::Background::Type, std::vector<Double_t> >::iterator it2 = fHistBackground.begin();
  while ( it2 != fHistBackground.end() ) {
    for (unsigned int i=0; i < nbin; i++) (it2->second).at(i) *= scale_factor;
      it2++;
  }
  fUnderflow *= scale_factor;
  fOverflow  *= scale_factor;
}

void Sample::Update()
{
  if (fBins.size() == 0 ) return;
  //Consts::EUpdateType utype = Consts::kNoUpdate;

  // misc
  MsgDebug(2, fName);
  StartWatch("Sample::Update " + fName);

  for (unsigned int i=0;i<fWC.size();i++) {
    if ( fWC.at(i)->KindOfUpdate(fWeightParMap) == Consts::kScaleUpdate) {
      Scale(fWC.at(i)->GetRescalingFactor(fWeightParMap));
      std::map<std::string, Double_t > p = fWC.at(i)->GetParMap();
      std::map<std::string, Double_t >::const_iterator pit = p.begin();
      while ( pit != p.end() ) {
        fWeightParMap[pit->first] = pit->second;
        MsgDebug(2, "fWeightParMap[" + pit->first + "] has been scaled" );
        pit++;
      }
    }

    if ( fWC.at(i)->KindOfUpdate(fWeightParMap) == Consts::kFullUpdate) {
      MsgDebug(2, fWC.at(i)->GetName() + " needs full update (sample " + fName + ")");
      //FillHistogram(true,true);
      if(fWC.at(i)->GetNParameters()<=2){      //currently don't save for 3 parameters
        FillHistogram(true,true);
      }
      else{
        FillHistogram(true,false);
      }
      StopWatch("Sample::Update " + fName);
      return;
    }
    if ( fWC.at(i)->KindOfUpdate(fWeightParMap) == Consts::kNoUpdate) {
      MsgDebug(2, fWC.at(i)->GetName() + " needs no update (sample " + fName + ")");
    }
  } // end of for(fWC)
  StopWatch("Sample::Update " + fName);

}

bool Sample::NeedUpdate()
{
  //Check if anything changed
  for (unsigned int i=0; i<fWC.size(); i++)
    if ( fWC.at(i)->ParametersChanged() ) fNeedUpdate = true;
  
  return fNeedUpdate;
} 

Consts::Background::Type Sample::WhichBackground(Event &e)
{
  for ( Int_t i = Consts::BackgroundIter(Consts::Background::kFirst);i<=Consts::BackgroundIter(Consts::Background::kLast); i++ ) {
    if ( IsBackground(e, Consts::BackgroundIterToEnum(i)) ) return Consts::BackgroundIterToEnum(i);
  }
  MsgError("No background is found!");
  return Consts::Background::kLast;

}

bool Sample::IsBackground(Event &e, Consts::Background::Type b)
{

  switch ( b ) {
    case Consts::Background::kIsPi0:
    if ( e.ispi0() == 1 && e.isdirt() == 0) return true;
    break;
    case Consts::Background::kIsDirt:
    if ( e.isdirt() == 1 ) return true;
    break;
    case Consts::Background::kIsDelta:
    if ( e.nuchan() == 94 && e.isdirt() == 0 ) return true;
    break;
    case Consts::Background::kPiPlus:
    if ( e.parid() ==  Consts::EnumToParticleID(Consts::kPiPlus)  && e.ispi0() == 0 && e.isdirt() == 0 && e.nuchan() != 94 && 
      (e.inno() == 3 || e.inno() == 4) ) return true;
      break;
    case Consts::Background::kPiMinus:
    if ( e.parid() ==  Consts::EnumToParticleID(Consts::kPiMinus) && e.ispi0() == 0 && e.isdirt() == 0 && e.nuchan() != 94 &&
     (e.inno() == 3 || e.inno() == 4) ) return true;
      break;
    case Consts::Background::kKPlus:
    if ( e.parid() ==  Consts::EnumToParticleID(Consts::kKPlus)   && e.ispi0() == 0 && e.isdirt() == 0 && e.nuchan() != 94 &&
     (e.inno() == 3 || e.inno() == 4) ) return true;
      break;
    case Consts::Background::kKMinus:
    if ( e.parid() ==  Consts::EnumToParticleID(Consts::kKMinus)  && e.ispi0() == 0 && e.isdirt() == 0 && e.nuchan() != 94 &&
     (e.inno() == 3 || e.inno() == 4) ) return true;
      break;
    case Consts::Background::kK0L:
    if ( e.parid() ==  Consts::EnumToParticleID(Consts::kK0L)     && e.ispi0() == 0 && e.isdirt() == 0 && e.nuchan() != 94 &&
     (e.inno() == 3 || e.inno() == 4) ) return true;
      break;
    case Consts::Background::kMuPlus:
    if ( e.parid() ==  Consts::EnumToParticleID(Consts::kMuPlus)  && e.ispi0() == 0 && e.isdirt() == 0 && e.nuchan() != 94 &&
     (e.inno() == 3 || e.inno() == 4) ) return true;
      break;
    case Consts::Background::kMuMinus:
    if ( e.parid() ==  Consts::EnumToParticleID(Consts::kMuMinus) && e.ispi0() == 0 && e.isdirt() == 0 && e.nuchan() != 94 &&
     (e.inno() == 3 || e.inno() == 4) ) return true;
      break;
    case Consts::Background::kUnknown:
    bool is_unknown=true;
    for ( Int_t i = Consts::BackgroundIter(Consts::Background::kFirst);i<Consts::BackgroundIter(Consts::Background::kLast); i++ ) {
      if ( Consts::BackgroundIterToEnum(i) != Consts::Background::kUnknown && 
        IsBackground(e, Consts::BackgroundIterToEnum(i)) ) is_unknown = false;
    }
  return is_unknown;
  break;
};

return false;  
}
void Sample::Rebin(std::vector<Int_t> rebin_vector)
{
  //First check rebin_vector
  //Vector should have same number of elements as the current number of bins
  //If rebin_vector is 0 the bin will be excluded

  for (unsigned int i=0; i<rebin_vector.size(); i++) rebin_vector.at(i)=abs(rebin_vector.at(i));
    Int_t badvector = 0;
  if ( rebin_vector.size() != fHistContent.size() ) badvector = 1;
  Int_t last_non_zero_val=0;
  Int_t last_non_zero_bin=0;
  for (unsigned int i=0; i<rebin_vector.size(); i++) {
    if ( rebin_vector.at(i) < 0 ) badvector += 100; 
    if (!badvector && rebin_vector.at(i) != 0) {
      if ( rebin_vector.at(i) != last_non_zero_val && rebin_vector.at(i) != last_non_zero_val + 1 ) 
        badvector += 10000;

      last_non_zero_val = rebin_vector.at(i);
      last_non_zero_bin = i;
    }
    else if ( last_non_zero_val && (i<rebin_vector.size()-1) && rebin_vector[i+1] != 0 ) badvector += 1000000;
  }
  if ( badvector ) {
    MsgWarning(MsgLog::Form("Sample::Invalid rebin vector. Error code = %d", badvector ));
    MsgWarning("Sample:: " + fName + " sample not rebinned." );
    return;
  }
  Int_t n_new_bins = last_non_zero_val;
  Int_t n_old_bins = fHistContent.size();

  //Form new bins vector
  std::vector<Double_t> newbins;
  Int_t last_rebin = 0;
  for (unsigned int i=0; i<fBins.size()-1;i++) {
    if ( rebin_vector.at(i) != 0 && rebin_vector.at(i) != last_rebin) {
      newbins.push_back(fBins.at(i));
      last_rebin = rebin_vector.at(i);
    }
  }
  if ( last_non_zero_bin ) newbins.push_back(fBins.at(last_non_zero_bin+1));

  fBins.clear();
  fNBins=newbins.size();
  fBins.resize(fNBins);
  for (unsigned int i=0;i<newbins.size();i++) fBins.at(i) = newbins[i];

  //If data is from ntuple just refill the histograms
  if (fDataFromNtuple) {
    FillVector();
    FillHistogram();
  } else {
    std::vector<Double_t> new_hist(n_new_bins);
    for ( Int_t i = 0; i < n_old_bins; i++ ) {
      if ( rebin_vector.at(i) != 0 ) {
       new_hist[rebin_vector.at(i)] += fHistContent[i];
     }
   }
   fHistContent.clear();
   fHistContent = new_hist;
 }
 if (fFastHist.size()>0) {
   fFastHist.clear();
   MsgDebug(2, "Cleared fast hist map after rebinning ");
 }




}

Int_t Sample::FindBin(Double_t val)
{
  if ( val < fBins.at(0) )                   return Consts::kUnderflow;
  else if ( val >= fBins.at(fBins.size()-1) ) return Consts::kOverflow;

  /*  
  Int_t bin=0;
  while (val >= fBins.at(bin)) bin++;
  bin--;
  return bin;
  */

  Double_t *mybin=std::lower_bound(&fBins.at(0),&fBins.at(0)+fNBins,val);
  if ( (mybin != &fBins.at(0) + fNBins) && (*mybin == val) )
    return (mybin - &fBins.at(0));
  else
    return ( mybin - &fBins.at(0) - 1);

}

TH1D* Sample::GetHistogram()
{
  //  UInt_t Nbins=fBins.size();
  if (fNBins>0) {
    Float_t bins[fNBins];
    for (UInt_t i=0;i<fNBins;i++) bins[i]=fBins.at(i);
      TH1D* h=new TH1D(fName.c_str(),fName.c_str(),fNBins-1,bins);
    for (UInt_t i=0;i<fNBins-1;i++) h->SetBinContent(i+1,fHistContent.at(i));
      h->SetBinContent(0,fUnderflow);
    h->SetBinContent(fNBins+1,fOverflow);
    return h;
  }
  return 0;
}

TH1D* Sample::GetHistogram(std::string name, std::vector<Double_t> vec, std::vector<Double_t> err)
{

  //UInt_t Nbins=fBins.size();
  if (fNBins>0) {
    Float_t bins[fNBins];
    for (UInt_t i=0;i<fNBins;i++) bins[i]=fBins.at(i);
      TH1D* h=new TH1D(name.c_str(),name.c_str(),fNBins-1,bins);
    for (UInt_t i=0;i<fNBins-1;i++) {
      h->SetBinContent(i+1,vec.at(i));
      if (err.size() == vec.size()) h->SetBinError(i+1,err.at(i));
    }
    return h;
  }
  return 0;
}

void Sample::WriteHistogramsToFile(TFile* f, bool dump_bg_hist, string dir, string app)
{
  if (fBins.size() == 0 ) return;
  TDirectory* tmpd=gDirectory;
  gDirectory =  f->GetDirectory(dir.c_str());
  if(gDirectory==NULL)
    gDirectory =  f->GetDirectory("");
  TH1D* h=GetHistogram(fName, fHistContent, fHistError);
  string hName = h->GetName();
  if (h!=0) {
    if (app!=""){
      // remove the space and append app
      while(hName.find(" ")!=string::npos) 
        hName.replace(hName.find(" "), 1, "_");
      hName = hName + "_" + app;
    }
    h->Write(hName.c_str());
    delete h;
    h=0;
  }

  if (fDataFromNtuple && dump_bg_hist) {    
    for (Int_t biter=Consts::BackgroundIter(Consts::Background::kFirst); biter <= Consts::BackgroundIter(Consts::Background::kLast); biter++) {
      map<Consts::Background::Type, std::vector<Double_t> >::iterator bit = fHistBackground.find(Consts::BackgroundIterToEnum(biter));
      if (bit != fHistBackground.end()) {
       std::vector<Double_t> dummy;
       TH1D* h=GetHistogram(Form("%s %s",fName.c_str(), Consts::GetBackgroundName(bit->first).c_str()), bit->second, dummy);
       h->Write();
       delete h;
       h=0;
     }
   }
 }

 gDirectory = tmpd;
}

void Sample::Dump()
{
  if (fBins.size() == 0) return;
  ostringstream oss;
  oss << endl;
  oss << "****************************************************************************************"<<endl;
  oss << "Sample name: "  << fName    << endl;
  oss << "Input file: "   << fInputFileName << endl;
  oss << "fFastHist.size(): " << fFastHist.size() << endl;
  oss << "Events read: "  << fNevents;
  if (fDownsample<1 && fDownsample>0)  
    oss<<" ("<<fDownsample<<" of the total number of events in the ntuple)";
  oss    << endl; 

  oss << "Weight calculators: " << endl;
  for (std::vector<std::shared_ptr<WeightCalc>>::const_iterator it=fWC.begin(); it != fWC.end(); it++)
    (*it)->Dump();

  for (auto const& filter : fFilter) filter->Dump();
  oss <<endl;

  if (fVerbosity < 2) return;

  
  oss<< setw(10) << fName << " bins";
  for (unsigned int i=0; i<fBins.size(); i++) {oss<<setw(10) << setprecision(4)<<fBins.at(i);}
    oss<<endl;
  for (unsigned int i=0; i<fBins.size()*10+30; i++) oss<<"-";
    oss<<endl;
  oss << setw(15) << fName<<"     ";
  for (auto const& binContent : fHistContent) { oss<< setw(10) << setprecision(4)<<binContent;}
    oss <<endl;

  if (fVerbosity < 3) return;

  oss <<"---------------"<<endl;
  for (Int_t piter=Consts::ParticleIter(Consts::kFirst); piter <= Consts::ParticleIter(Consts::kLast); piter++) {
    map<Consts::ParticleCode, std::vector<Double_t> >::iterator pit = fHistParent.find(Consts::ParticleIterToEnum(piter));
    if (pit != fHistParent.end()) {
      oss << setw(10) << Consts::GetParticleName(pit->first)<<"          ";
      for (unsigned int i=0; i<fHistContent.size(); i++) {oss<< setw(10) << setprecision(4)<<(pit->second)[i];}
        oss <<endl;
    }
  }
  oss <<"---------------"<<endl;
  for (Int_t biter=Consts::BackgroundIter(Consts::Background::kFirst); biter <= Consts::BackgroundIter(Consts::Background::kLast); biter++) {
    map<Consts::Background::Type, std::vector<Double_t> >::iterator bit = fHistBackground.find(Consts::BackgroundIterToEnum(biter));
    if (bit != fHistBackground.end()) {
      oss << setw(10) << Consts::GetBackgroundName(bit->first)<<"          ";
      for (unsigned int i=0; i<fHistContent.size(); i++) {oss<< setw(10) << setprecision(4)<<(bit->second)[i];}
        oss <<endl;
    }
  }
  
  for (unsigned int i=0; i<fBins.size()*10+30; i++) oss<<"-";
    oss <<endl;
  oss <<"Overflow bin  ( E > "<<fBins.at(fBins.size()-1)<<" ) = "<<fOverflow<<endl;
  oss <<"Underflow bin ( E < "<<fBins.at(0)             <<" ) = "<<fUnderflow<<endl;
  oss << "****************************************************************************************"<<endl;
  MsgInfo(oss.str());
  oss.str("");
}
