#ifndef FITMASTER_H
#define FITMASTER_H

#include <string>
#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <memory>
#include "ErrorMatrixROOT.h"
#include "Util.h"
#include "SampleROOT.h"
#include "MsgLog.h"

#include "WeightCalc.h"
#include "Algo.h"

#include "TTree.h"
class FitMaster {

public :
  static FitMaster* Instance();

  enum EFitType {
    kChi2,
    kLikelihood
  };

  void         AddFracErrorMatrix(std::shared_ptr<ErrorMatrix> matrix);
  std::shared_ptr<ErrorMatrix> GetFracErrorMatrix(std::string name) {return (fFracErrorMatrix.find(name))->second;};
  std::vector<std::string>     GetListOfFracErrorMatrixNames();

  void    AddDataSample(std::shared_ptr<Sample> data);
  std::shared_ptr<Sample> GetDataSample(std::string name) 
  {
    for(auto const& DataSample : fDataSampleList){
      if(DataSample->GetName() == name) 
        return DataSample;
    }
    return 0;    
    //std::list<Sample* >::const_iterator it=fDataSampleList.begin();
    //while ( (*it)->GetName() != name && it != fDataSampleList.end() ) it++;
    //if ( (*it)->GetName() != name ) return 0;
    //return (*it);
  };

  void    AddMCSample(std::shared_ptr<Sample> mc);
  std::shared_ptr<Sample> GetMCSample(std::string name) 
  {
    for(auto const& MCSample : fMCSampleList){
      if(MCSample->GetName() == name) 
        return MCSample;
    }
    return 0;
    //std::list<Sample* >::const_iterator it=fMCSampleList.begin();
    //while ( (*it)->GetName() != name && it != fMCSampleList.end() ) it++;
    //if ( (*it)->GetName() != name ) return 0;
    //return (*it);
  };

  std::list<std::shared_ptr<Sample> > GetListOfSamples(std::string type="Signal");

  void SetDataPOT(Double_t pot) {fDataPOT=pot;};
  void SetTotalPOT(Double_t pot) {fTotPOT=pot;};
  void AddFakeDataToRealData(bool val) {fAddFakeDataToRealData = val;};

  void RemoveDataSample(std::string name) 
  {

    std::list<std::shared_ptr<Sample> >::iterator it=fDataSampleList.begin();
    while ( (*it)->GetName() != name && it != fDataSampleList.end() ) it++;
    if ( (*it)->GetName() == name ) {
      fDataSampleList.erase(it);
    } else {
      MsgWarning(MsgLog::Form("Cannot remove nonexisting %s sample!", name.c_str()));
    }
  }

  void AddAlgoToRun(std::shared_ptr<Algo> algo) {algo->SetVerbosity(fVerbosity);fRunAlgo[algo->GetName()]=algo;};

  void BuildEventVector();
  void BuildDataVector();
  void BuildFullErrorMatrix();

  void BuildCollapsedEventVector();
  void BuildCollapsedErrorMatrix();

  std::vector<Double_t > GetEventVector() {return fEventVector;};
  void RecalculateTotalFracErrorMatrix();

  void SetBinning(std::vector<Double_t> bins) {fMCBins = bins;};
  void SetRebinVector(std::vector<Int_t> rebin_vector) {fRebinVector = rebin_vector;};
  void Rebin(std::map<std::string, std::vector<Int_t> > binmap);
  void InvertCollapsedErrorMatrix();
  void UpdateHistograms();

  void SetParameter(std::string name, Double_t par);

  Double_t GetChi2();
  Double_t GetLSNDChi2();

  const ErrorMatrix GetFracErrorMatrix()      {
    ErrorMatrix tempMatrix;
    tempMatrix.CopyFrom(fTotalFracErrorMatrix);
    return tempMatrix;
  };
  const ErrorMatrix GetCollapsedErrorMatrix() {
    ErrorMatrix tempMatrix;
    tempMatrix.CopyFrom(fCollapsedErrorMatrix);
    return tempMatrix;
  };

  void CorrectMSstat(std::string matrix_name, 
   std::map<std::string, Double_t> ms_to_data_pot);

  void AddSignalStatError(bool val) {fAddSignalStatError = val;};
  void AddStatError(bool val) {fAddStatError = val;};

  void AddFullOscStat();
  void AddStatError();
  void RemoveFullOscStat();

  void CalculateErrors(std::vector<std::string> parname, std::vector<Double_t> par, std::string name="", TFile* fout=0);

  void Run();
  TFile* GetFoutFake() {return foutFake;}; // get the fout for fake data fit
  TTree* GetTreeFake() {return treeFake;}; // get the tree for fake data fit

  void DoFakeDataFits(bool val) {fDoFakeDataFits=val;};
  void SetNumberOfFakeDataFits(UInt_t n) {fNfakeDataFits=n;};
  UInt_t GetNumberOfFakeDataFits() {return fNfakeDataFits;};
  UInt_t GetCurrentFakeDataFitN() {return fCurrentFakeDataFitN;};
  void SetFakeDataParameter(std::string name, Double_t val);
  Double_t GetFakeDataParameter(std::string name) {return fFakeDataParameter.find(name)->second;}

  void SetFakeDataRandomSeed(UInt_t n) {fFakeDataRandomSeed=n;};
  UInt_t GetFakeDataRandomSeed() {return fFakeDataRandomSeed;};

  bool IsFakeDataFit() {return fDoFakeDataFits;};
  bool IsLikelihoodFit() {return fFitType==kLikelihood;};

  void  SetOutputFileStem(std::string s);
  std::string GetOutputFileStem() { return fOutputFileStem;};

  void   SetVerbosity(Int_t val);
  Int_t  GetVerbosity() {return fVerbosity;};
  void   Dump();
  void   SetLSNDWeightCalc(std::shared_ptr<WeightCalc> wc);

  void SetFitType(std::string fit_type) {
    if (fit_type == "Chi2") fFitType=kChi2;
    else if (fit_type == "Likelihood") fFitType=kLikelihood;
    else std::cout <<"Fit type "<<fit_type<<" not recognized. Will do "<<GetFitType()<<" fit"<<std::endl;
  };
  std::string GetFitType() {
    std::string fit_type;
    if (fFitType==kChi2) fit_type="Chi2";
    else if (fFitType==kLikelihood) fit_type="Likelihood";
    return fit_type;
  };

  Double_t GetLogDeterminant() {return fLogDeterminant;};

  std::shared_ptr<WeightCalc> GetWeightCalc(std::string name) {return std::shared_ptr<WeightCalc>((fWeightCalc.find(name)->second)->Clone());};
  std::shared_ptr<Algo>       GetAlgo(std::string name)       {return fAlgo.find(name)->second;};

  friend void Util::AddMonteCarloStatError(FitMaster* fit_master);

private:
  FitMaster();
  virtual ~FitMaster();

  static FitMaster* fInstance;

  EFitType fFitType;

  void Register(std::shared_ptr<WeightCalc> wc) {fWeightCalc[wc->GetName()] = wc;};
  void Register(std::shared_ptr<Algo> algo)     {fAlgo[algo->GetName()] = algo;};

  bool isTotalRecent;
  bool fAddSignalStatError;
  bool fAddStatError;
  bool fDoFakeDataFits;

  UInt_t fNfakeDataFits;
  UInt_t fCurrentFakeDataFitN;
  std::map<std::string, Double_t> fFakeDataParameter;
  UInt_t fFakeDataRandomSeed;
  bool fAddFakeDataToRealData;
  Double_t fTotPOT;
  Double_t fDataPOT;

  Int_t fVerbosity;
  std::string fOutputFileStem;

  std::list<std::shared_ptr<Sample> >  fMCSampleList;
  std::list<std::shared_ptr<Sample> >  fDataSampleList;
  std::map<std::string, std::string >  fBackgroundSignal;
  std::map<std::string, std::shared_ptr<ErrorMatrix> > fFracErrorMatrix;  // input matrices from XML file with name as input
  std::shared_ptr<ErrorMatrix> fTotalFracErrorMatrix;   //  sum of all fFracErrorMatrix
  std::shared_ptr<ErrorMatrix> fTotalFullErrorMatrix;   //  fTotalFracErrorMatrix[i][j] * fEventVector[i] * fEventVector[j]           
  std::shared_ptr<ErrorMatrix> fCollapsedErrorMatrix;
  std::shared_ptr<ErrorMatrix> fInvertedCollapsedErrorMatrix;

  Double_t fLogDeterminant;

  std::vector<Double_t >  fDataVector;
  std::vector<Double_t >  fEventVector;
  std::vector<Double_t >  fSumw2Vector;
  std::vector<Double_t >  fCollapsedEventVector;
  std::vector<Int_t >     fRebinVector;
  std::map<std::string, std::vector<Int_t> > fRebinMap;
  std::vector<Double_t >  fMCBins;
  std::vector<Double_t >  fDataBins;

  UInt_t fNdataBins;

  std::map<std::string, std::shared_ptr<Algo> >        fRunAlgo;  
  std::map<std::string, std::shared_ptr<Algo> >        fAlgo;
  std::map<std::string, std::shared_ptr<WeightCalc> >  fWeightCalc;
  //  std::map<std::string, FilterEvent* > fFilterEvent;

  // for fake data
  TFile* foutFake;
  TTree* treeFake;
  std::map<std::string, std::vector<Double_t> >        fCollapsedEventVectorLibrary;
  std::map<std::string, std::shared_ptr<ErrorMatrix> > fTotalFullErrorMatrixLibrary;
  std::map<std::string, std::shared_ptr<ErrorMatrix> > fCollapsedErrorMatrixLibrary;
  std::map<std::string, Double_t >                     fLogCollapsedErrorMatrixLibrary;
  std::map<std::string, std::shared_ptr<ErrorMatrix> > fInvertedCollapsedErrorMatrixLibrary;
  std::vector<std::string> fParNames;
  std::vector<double>      fParValues;
  bool                     fReadFromLibrary;
  std::string              fLibraryTicket; // consist of each parameter values

  // include LSND ad-hoc way
  bool                        fIncludeLSND;
  std::shared_ptr<WeightCalc> fLSNDWeightCalc;
};

#endif
