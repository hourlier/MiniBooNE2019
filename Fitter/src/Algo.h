#ifndef ALGO_H
#define ALGO_H

#include <iostream>
#include <vector>
#include <string>
#include "Rtypes.h"

class FitMaster;
class TH2F;
class TH3F;

class Algo {

 public:
  Algo(FitMaster* fm);
  virtual ~Algo();

  virtual void Run() = 0;
  virtual void SetOption(std::string opt,bool val) {};
  void  SetVerbosity(Int_t val) {fVerbosity = val;};
  

  std::string GetName() {return fName;};
  std::vector<std::string> GetParNames() {return fParName;};
  TH2F* Get2dHist(std::string name, std::string xparname, std::string yparname, bool isxlog = true, bool isylog = true);
  TH3F* Get3dHist(std::string name, std::string xparname, std::string yparname, std::string zparname, bool isxlog = true, bool isylog = true, bool iszlog = true);
  void AddParameter(std::string name, Int_t ngrid, Double_t min, Double_t max, bool logscale=false);
  void SetParameterNull(std::string name, Double_t null); //used to evaluate null matrix

  void Dump();

 protected:
  std::string fName;
  Int_t fVerbosity;
  
  Double_t fMinChi2;
  std::vector<std::string> fParName;
  std::vector<bool>        fLogScale;
  std::vector<Int_t>       fNbins;
  std::vector<Double_t>    fParMin;
  std::vector<Double_t>    fParMax;
  std::vector<Double_t>    fPar;
  std::vector<Int_t>       fBin;
  std::vector<Double_t>    fParBestFit;
  std::vector<Double_t>    fParNull;

  FitMaster* fFitMaster;
};
#endif
