



#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include "cfortran.h"
#include <packlib.h>
#include <math.h>
#include <algorithm>

#include "Event.h"
#include "Consts.h"
#include "Sample.h"

#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TRandom3.h"
#include "TDirectory.h"
#include "THbookFile.h"
#include "TSystem.h"





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

Sample* Sample::Clone()
{
  Sample* new_sample = new Sample();
  new_sample->SetName(fName);
  new_sample->SetInputNtuple(fInputFileName);
  new_sample->SetNtupleID(fNtupleID);
  new_sample->SetVerbosity(fVerbosity);
  new_sample->SetBins(fBins);
  for (int i=0; i<fWC.size(); i++)     
    new_sample->AddWeightCalc(fWC[i]->Clone());
  for (int i=0; i<fFilter.size(); i++) 
    new_sample->AddFilter(fFilter[i]);

  new_sample->SetDownsample(fDownsample);

  new_sample->FillVector();
  new_sample->FillHistogram();

  return new_sample;
}

void Sample::AddWeightCalc(WeightCalc *wc)
{ 
  fWC.push_back(wc); 
  fNeedUpdate = true;
  std::map<std::string, Double_t > parmap = wc->GetParMap();
  fWeightParMap.insert(parmap.begin(), parmap.end() );
}

void Sample::FillVector()
{
  std::string fIndir  = "INPUT";
  int lrec=1024;
  int istat=0;

  char* input_file = const_cast<char*> (fInputFileName.c_str());
  HROPEN(20, strdup(fIndir.c_str()), input_file, "P", lrec, istat);
  if (istat != 0) {
    cout << "ERROR: Error opening input file ... ("<<fInputFileName<<")" << endl;
    exit(1);
  }
  HRIN(0, 999999, 0);
  int ierr=0;
  Int_t nevents;

  HCDIR(strdup(("//"+fIndir).c_str()), " ");
  HNOENT(fNtupleID, nevents);

  Int_t IDN=0;
  float x[11];

  fData.clear();

  Int_t nskip=0;
  for ( Int_t ifilt=0; ifilt < fFilter.size(); ifilt++ ) 
    fFilter[ifilt]->Reset();

  TRandom3 rnd;
  rnd.SetSeed(1);
  for ( Int_t i = 1 ; i <= nevents ; i++ ) {
    HGN(fNtupleID, IDN, i, x, ierr);
    Event mb;
    if ( ierr != 0 )  {
      cout << "ERROR:: Can't read event " << i <<endl;
    } else {
      mb.iflux   ( Int_t(x[0])    );  // QQ: What's iflux?
      mb.ibkgd   ( Int_t(x[1])    );  // QQ: The same as the background?
      mb.nuchan  ( Int_t(x[2])    );
      mb.inno    ( Int_t(x[3])    );
      mb.enugen  ( Double_t(x[4]) );
      mb.energy  ( Double_t(x[5]) );
      mb.nuleng  ( Double_t(x[6]) );
      mb.parid   ( Int_t(x[7])    );
      mb.wgt     ( Double_t(x[8]) );
      mb.ispi0   ( Int_t(x[9])    );
      mb.isdirt  ( Int_t(x[10])   );

      bool keep = true;
      for ( Int_t ifilt=0; ifilt < fFilter.size(); ifilt++ ) {
        if ( fFilter[ifilt]->Cut(mb) ) keep = false;
      }
      if (fDownsample<1 && fDownsample>0) {
        if (rnd.Uniform()>fDownsample) keep=false; 
	       mb.wgt(mb.wgt()/fDownsample); //boost weight of kept events
      }
      if ( keep ) fData.push_back(mb);
      else nskip++;
    }
  }
  HREND(strdup((""+fIndir).c_str()));
  fNevents = fData.size();
  if ( nevents != nskip+fNevents ) {
    cout <<"Warning:: "<<nevents << " found in "<<fInputFileName<<" but only "<<fNevents+nskip<<" read!"<<endl;
  }
}

void Sample::FillHistogram(bool useweights, bool fast_hist)
{
  //Function fills the histogram using the data input from the ntuple 
  // useweights = true, fast_hist = false by default

  if (fBins.size() <= 0 ) {
    fHistContent.clear();
    fHistContent.resize(0);
    fHistSumw2.clear();
    fHistSumw2.resize(0);
    return;
  }

  fHistContent.clear();
  fHistContent.resize(fBins.size()-1);
  fHistSumw2.clear();
  fHistSumw2.resize(fBins.size()-1);

  fUnderflow = 0;
  fOverflow = 0;

  fWeightParMap.clear();
  std::string wghlist="";
  for (unsigned int i=0;i<fWC.size();i++){
    std::map<std::string, Double_t > parmap = fWC[i]->GetParMap();
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
      //  cout <<"Found "<<wghlist<<endl;
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
        weight_calc *= fWC[i]->GetWeight(mb);
      }
    }

    if      ( bin == Consts::kUnderflow ) fUnderflow += mb.wgt()*weight_calc;
    else if ( bin == Consts::kOverflow  ) fOverflow  += mb.wgt()*weight_calc;
    else {
      fHistContent[bin] += mb.wgt()*weight_calc;
      fHistSumw2[bin]   += mb.wgt()*weight_calc*mb.wgt()*weight_calc;
    }
  }
  fDataFromNtuple = true;
  fNeedUpdate = false;

  if (fast_hist ) fFastHist[wghlist]=fHistContent;

  if (fFillBkgHists) FillBkgHists();
}

void Sample::FillBkgHists(bool useweights)
{
  //Function fills the histogram using the data input from the ntuple 
  if (fBins.size() <= 0 ) {
    return;
  }
  fHistParent.clear();
  const Int_t nparents=8;
  std::vector<Double_t> vec[nparents];
  std::pair<Consts::ParticleCode, std::vector<Double_t> > p[nparents];
  for (int i=0;i<nparents;i++) vec[i].resize(fBins.size()-1);
  p[0] = std::pair<Consts::ParticleCode, std::vector<Double_t> >(Consts::kPiPlus,  vec[0]);
  p[1] = std::pair<Consts::ParticleCode, std::vector<Double_t> >(Consts::kPiMinus, vec[1]);
  p[2] = std::pair<Consts::ParticleCode, std::vector<Double_t> >(Consts::kKPlus,   vec[2]);
  p[3] = std::pair<Consts::ParticleCode, std::vector<Double_t> >(Consts::kKMinus,  vec[3]);
  p[4] = std::pair<Consts::ParticleCode, std::vector<Double_t> >(Consts::kK0L,     vec[4]);
  p[5] = std::pair<Consts::ParticleCode, std::vector<Double_t> >(Consts::kMuPlus,  vec[5]);
  p[6] = std::pair<Consts::ParticleCode, std::vector<Double_t> >(Consts::kMuMinus, vec[6]);
  p[7] = std::pair<Consts::ParticleCode, std::vector<Double_t> >(Consts::kUnknown, vec[7]);
  for (int i=0;i<nparents;i++) fHistParent.insert(p[i]);

  fHistBackground.clear();
  for ( Int_t i = Consts::BackgroundIter(Consts::Background::kFirst);i<=Consts::BackgroundIter(Consts::Background::kLast); i++ ) {
    std::vector<Double_t> bvec;
    bvec.resize(fBins.size()-1);
    fHistBackground.insert(std::pair<Consts::Background::Type, std::vector<Double_t> > (Consts::BackgroundIterToEnum(i), bvec));
    //    if ( IsBackground(e, Consts::BackgroundIterToEnum(i)) ) return Consts::BackgroundIterToEnum(i);
  }
  /*
  fWeightParMap.clear();
  std::string wghlist="";
  for (unsigned int i=0;i<fWC.size();i++){
    std::map<std::string, Double_t > parmap = fWC[i]->GetParMap();
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
       weight_calc *= fWC[i]->GetWeight(mb);
      }
    }

    if ( bin != Consts::kUnderflow && bin != Consts::kOverflow) {
      map<Consts::ParticleCode, std::vector<Double_t>  >::iterator pit = fHistParent.find(Consts::ParticleIDtoEnum(mb.parid()));
      if ( pit != fHistParent.end() ) {
        fHistParent[Consts::ParticleIDtoEnum(mb.parid())][bin] += mb.wgt()*weight_calc;
      } else 
        cout << "No histogram available for parent type "<<mb.parid()<<endl;

      map<Consts::Background::Type, std::vector<Double_t>  >::iterator pit2 = fHistBackground.find(WhichBackground(mb));
      if ( pit2 != fHistBackground.end() ) {
        fHistBackground[WhichBackground(mb)][bin] += mb.wgt()*weight_calc;
      } else 
        cout << "No histogram available for background "<<Consts::GetBackgroundName(WhichBackground(mb))<<endl;
    }  
  }
}

void Sample::FillHistogram(std::vector<Double_t> data)
{
  //function fills the histogram using provided vector
  //the binning has to be provided prior to calling this function using SetBins()
  if (data.size() != fBins.size()-1) {
    cout << "Warning::Error while filling histogram for "<<fName<<endl;
    cout << "         Data vector size is "<<data.size()<<" while number of bins is "<<fBins.size()-1<<endl;
  }

  fHistContent.clear();
  fHistContent.resize(fBins.size()-1);
  fHistSumw2.clear();
  fHistSumw2.resize(fBins.size()-1);

  for (int i = 0; i<data.size();i++) {
    fHistContent[i] = data[i];
    fHistSumw2[i]   = data[i];
  }
  fDataFromNtuple = false;
  fNeedUpdate = false;
}

void Sample::SetErrors(std::vector<Double_t> err)
{
  if ( err.size() != fHistContent.size() ) {
    cout <<"Size mismatch between error vector ("<<err.size()<<") and data vector ("
    <<fHistContent.size()<<")"<<endl;
    cout <<"Errors for sample "<<fName<<" not set correctly!"<<endl;
    return;
  }

  fHistError=err;
}

void Sample::Scale(Double_t scale_factor)
{
  unsigned nbin = fHistContent.size();
  for (unsigned int i=0; i < nbin; i++) {

    fHistContent[i] *= scale_factor;
    fHistSumw2[i]   *= scale_factor;
  }
  std::map<Consts::ParticleCode, std::vector<Double_t> >::iterator it = fHistParent.begin();
  while ( it != fHistParent.end() ) {
    for (unsigned int i=0; i < nbin; i++) (it->second)[i] *= scale_factor;
      it++;
  }
  std::map<Consts::Background::Type, std::vector<Double_t> >::iterator it2 = fHistBackground.begin();
  while ( it2 != fHistBackground.end() ) {
    for (unsigned int i=0; i < nbin; i++) (it2->second)[i] *= scale_factor;
      it2++;
  }
  fUnderflow *= scale_factor;
  fOverflow  *= scale_factor;
}

void Sample::Update()
{
  if (fBins.size() == 0 ) return;
  Consts::EUpdateType utype = Consts::kNoUpdate;

  for (unsigned int i=0;i<fWC.size();i++) {
    if ( fWC[i]->KindOfUpdate(fWeightParMap) == Consts::kScaleUpdate) {
      Scale(fWC[i]->GetRescalingFactor(fWeightParMap));
      std::map<std::string, Double_t > p = fWC[i]->GetParMap();
      std::map<std::string, Double_t >::const_iterator pit = p.begin();
      while ( pit != p.end() ) {
       fWeightParMap[pit->first] = pit->second;
       pit++;
     }
   }

   if ( fWC[i]->KindOfUpdate(fWeightParMap) == Consts::kFullUpdate) {
      //      cout <<fWC[i]->GetName()<<" needs full update (sample "<<fName<<")"<<endl;
    FillHistogram(true,true);
    return;
  }
}
}

bool Sample::NeedUpdate()
{
  //Check if anything changed
  for (unsigned int i=0; i<fWC.size(); i++)
    if ( fWC[i]->ParametersChanged() ) fNeedUpdate = true;
  
  return fNeedUpdate;
} 

Consts::Background::Type Sample::WhichBackground(Event &e)
{
  for ( Int_t i = Consts::BackgroundIter(Consts::Background::kFirst);i<=Consts::BackgroundIter(Consts::Background::kLast); i++ ) {
    if ( IsBackground(e, Consts::BackgroundIterToEnum(i)) ) return Consts::BackgroundIterToEnum(i);
  }
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

  for (Int_t i=0; i<rebin_vector.size(); i++) rebin_vector[i]=abs(rebin_vector[i]);
    Int_t badvector = 0;
  if ( rebin_vector.size() != fHistContent.size() ) badvector = 1;
  Int_t last_non_zero_val=0;
  Int_t last_non_zero_bin=0;
  for (Int_t i=0; i<rebin_vector.size(); i++) {
    if ( rebin_vector[i] < 0 ) badvector += 100; 
    if (!badvector && rebin_vector[i] != 0) {
      if ( rebin_vector[i] != last_non_zero_val && rebin_vector[i] != last_non_zero_val + 1 ) 
        badvector += 10000;

      last_non_zero_val = rebin_vector[i];
      last_non_zero_bin = i;
    }
    else if ( last_non_zero_val && i<rebin_vector.size()-1 && rebin_vector[i+1] != 0 ) badvector += 1000000;
  }
  if ( badvector ) {
    cout << "Sample::Invalid rebin vector. Error code = " << badvector << endl;
    cout << "Sample:: "<<fName<<" sample not rebinned."   << endl;
    return;
  }
  Int_t n_new_bins = last_non_zero_val;
  Int_t n_old_bins = fHistContent.size();

  //Form new bins vector
  std::vector<Double_t> newbins;
  Int_t last_rebin = 0;
  for (Int_t i=0; i<fBins.size()-1;i++) {
    if ( rebin_vector[i] != 0 && rebin_vector[i] != last_rebin) {
      newbins.push_back(fBins[i]);
      last_rebin = rebin_vector[i];
    }
  }
  if ( last_non_zero_bin ) newbins.push_back(fBins[last_non_zero_bin+1]);

  fBins.clear();
  fNBins=newbins.size();
  fBins.resize(fNBins);
  for (Int_t i=0;i<newbins.size();i++) fBins[i] = newbins[i];

  //If data is from ntuple just refill the histograms
  if (fDataFromNtuple) {
    FillVector();
    FillHistogram();
  } else {
    std::vector<Double_t> new_hist(n_new_bins);
    for ( Int_t i = 0; i < n_old_bins; i++ ) {
      if ( rebin_vector[i] != 0 ) {
       new_hist[rebin_vector[i]] += fHistContent[i];
     }
   }
   fHistContent.clear();
   fHistContent = new_hist;
 }
 if (fFastHist.size()>0) {
   fFastHist.clear();
   cout <<"Cleared fast hist map after rebinning "<<endl;
 }
}

Int_t Sample::FindBin(Double_t val)
{
  if ( val < fBins[0] )                   return Consts::kUnderflow;
  else if ( val >= fBins[fBins.size()-1] ) return Consts::kOverflow;

  /*  
  Int_t bin=0;
  while (val >= fBins[bin]) bin++;
  bin--;
  return bin;
  */

  Double_t *mybin=std::lower_bound(&fBins[0],&fBins[0]+fNBins,val);
  if ( (mybin != &fBins[0] + fNBins) && (*mybin == val) )
    return (mybin - &fBins[0]);
  else
    return ( mybin - &fBins[0] - 1);

}

TH1D* Sample::GetHistogram()
{
  //  UInt_t Nbins=fBins.size();
  if (fNBins>0) {
    Float_t bins[fNBins];
    for (Int_t i=0;i<fNBins;i++) bins[i]=fBins[i];
      TH1D* h=new TH1D(fName.c_str(),fName.c_str(),fNBins-1,bins);
    for (Int_t i=0;i<fNBins-1;i++) h->SetBinContent(i+1,fHistContent.at(i));
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
    for (Int_t i=0;i<fNBins;i++) bins[i]=fBins[i];
      TH1D* h=new TH1D(name.c_str(),name.c_str(),fNBins-1,bins);
    for (Int_t i=0;i<fNBins-1;i++) {
      h->SetBinContent(i+1,vec.at(i));
      if (err.size() == vec.size()) h->SetBinError(i+1,err.at(i));
    }
    return h;
  }
  return 0;
}

void Sample::WriteHistogramsToFile(TFile* f, bool dump_bg_hist)
{
  if (fBins.size() == 0 ) return;
  TDirectory* tmpd=gDirectory;
  gDirectory =  f->GetDirectory("");
  TH1D* h=GetHistogram(fName, fHistContent, fHistError);
  if (h!=0) {
    h->Write();
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

  cout << endl;
  cout << "****************************************************************************************"<<endl;
  cout << "Sample name: "  << fName    << endl;
  cout << "Input file: "   << fInputFileName << endl;
  cout << "Events read: "  << fNevents;
  if (fDownsample<1 && fDownsample>0)  
    cout<<" ("<<fDownsample<<" of the total number of events in the ntuple)";
  cout    << endl; 

  cout << "Weight calculators: " << endl;
  for (std::vector<WeightCalc*>::const_iterator it=fWC.begin(); it != fWC.end(); it++)
    (*it)->Dump();

  for (Int_t i=0; i<fFilter.size(); i++) {
    fFilter[i]->Dump();
  }
  cout <<endl;

  if (fVerbosity < 2) return;

  cout.width(10);
  cout<<fName<<" bins";
  for (int i=0; i<fBins.size(); i++) {cout.width(10); cout<<std::setprecision(4)<<fBins[i];}
    cout<<endl;
  for (int i=0; i<fBins.size()*10+30; i++) cout<<"-";
    cout<<endl;
  cout.width(15);
  cout <<fName<<"     ";
  for (int i=0; i<fHistContent.size(); i++) {cout.width(10); cout<<std::setprecision(4)<<fHistContent[i];}
    cout <<endl;

  if (fVerbosity < 3) return;

  cout <<"---------------"<<endl;
  for (Int_t piter=Consts::ParticleIter(Consts::kFirst); piter <= Consts::ParticleIter(Consts::kLast); piter++) {
    map<Consts::ParticleCode, std::vector<Double_t> >::iterator pit = fHistParent.find(Consts::ParticleIterToEnum(piter));
    if (pit != fHistParent.end()) {
      cout.width(10);
      cout <<Consts::GetParticleName(pit->first)<<"          ";
      for (int i=0; i<fHistContent.size(); i++) {cout.width(10); cout<<std::setprecision(4)<<(pit->second)[i];}
        cout <<endl;
    }
  }
  cout <<"---------------"<<endl;
  for (Int_t biter=Consts::BackgroundIter(Consts::Background::kFirst); biter <= Consts::BackgroundIter(Consts::Background::kLast); biter++) {
    map<Consts::Background::Type, std::vector<Double_t> >::iterator bit = fHistBackground.find(Consts::BackgroundIterToEnum(biter));
    if (bit != fHistBackground.end()) {
      cout.width(10);
      cout <<Consts::GetBackgroundName(bit->first)<<"          ";
      for (int i=0; i<fHistContent.size(); i++) {cout.width(10); cout<<std::setprecision(4)<<(bit->second)[i];}
        cout <<endl;
    }
  }
  
  for (int i=0; i<fBins.size()*10+30; i++) cout<<"-";
    cout <<endl;
  cout <<"Overflow bin  ( E > "<<fBins[fBins.size()-1]<<" ) = "<<fOverflow<<endl;
  cout <<"Underflow bin ( E < "<<fBins[0]             <<" ) = "<<fUnderflow<<endl;
  cout << "****************************************************************************************"<<endl;
}
