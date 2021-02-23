#ifndef DATASAMPLE_H
#define DATASAMPLE_H

#include <string>
#include <vector>
#include <map>
#include "Rtypes.h"
#include "Consts.h"
#include "WeightCalc.h"
#include "Filter.h"

class Event;
class TH1D;
class TFile;

class Sample {

 public :
  Sample();
  virtual ~Sample();

  void  FillVector();
  void  FillHistogram(bool useweights=true, bool fast_hist=false);
  void  FillBkgHists(bool useweights=true); //actually fills bkg hists
  void  FillBackgroundHists(bool val) {fFillBkgHists=val;}; //sets option to fill bkg hists
  void  FillHistogram(std::vector<Double_t> data);
  Int_t FindBin(Double_t val);
  TH1D* GetHistogram();

  TH1D* GetHistogram(std::string name, std::vector<Double_t> vec, std::vector<Double_t> err);

  void WriteHistogramsToFile(TFile* f, bool dump_bg_hist=true);

  void SetInputNtuple(std::string input_ntuple) {fInputFileName = input_ntuple;};
  void SetNtupleID(Int_t id)        {fNtupleID = id;};
  void SetBins(std::vector<Double_t> bins) {
    fBins.clear();
    fNBins=bins.size();
    fBins.resize(fNBins);
    for (int i=0; i<bins.size();i++) fBins[i]=bins[i];
    FillHistogram();
  };
  void AddWeightCalc(WeightCalc *wc);
  std::vector<WeightCalc*> GetWeightCalc() {return fWC;};

  void AddFilter(Filter *filter) {fFilter.push_back(filter);};
  std::vector<Filter*> GetFilter() {return fFilter;};

  bool NeedUpdate();
  Consts::EUpdateType KindOfUpdate();

  void SetErrors(std::vector<Double_t> err);
  std::vector<Double_t > GetBins() {return fBins;};
  Int_t    GetNbins() {return fHistContent.size();};
  Double_t GetBinContent(Int_t bin) {return fHistContent.at(bin);};
  Double_t GetError(Int_t bin) {return fHistError.at(bin);};
  Double_t GetBinSumw2(Int_t bin) {return fHistSumw2.at(bin);};

  void     Rebin(std::vector<Int_t> rebin_vector);
  
  void  SetVerbosity(Int_t val) {fVerbosity = val;};
  Int_t GetVerbosity()          {return fVerbosity;};
  void        Dump();
  void        SetName(std::string sample_name) {fName=sample_name;};
  std::string GetName() const {return fName;};
  void SetDownsample(Double_t val) {fDownsample=val;};

  void     Scale(Double_t scale_factor);
  bool IsBackground(Event &e, Consts::Background::Type b);
  Consts::Background::Type WhichBackground(Event &e);

  void  Update();

  Sample* Clone();

 private:
  std::string fName;
  std::string fInputFileName;
  Int_t fVerbosity;

  std::vector<Event>   fData;
  std::vector<Double_t> fHistContent;
  std::vector<Double_t> fHistError;
  std::map<Consts::ParticleCode, std::vector<Double_t> > fHistParent;
  std::map<Consts::Background::Type, std::vector<Double_t> > fHistBackground;
  std::vector<Double_t> fHistSumw2;
  std::vector<Double_t> fBins;
  UInt_t fNBins;
  Double_t fUnderflow;
  Double_t fOverflow;
  Double_t fScale;

  std::vector<WeightCalc*> fWC;
  std::map<std::string, Double_t > fWeightParMap;

  std::vector<Filter*> fFilter;

  Int_t fNtupleID;
  Int_t fNevents;

  bool fDataFromNtuple;
  Double_t fTolerance;
  bool fFirstFill;
  bool fNeedUpdate;
 
  bool fFillBkgHists;

  Double_t fDownsample; // fraction of events (from ntuple) kept in the sample

  std::map<std::string, std::vector<Double_t> > fFastHist;
};
#endif
