#ifndef ALGOFIT_H
#define ALGOFIT_H

#include "Algo.h"

class TFile;
class TTree;
class TH2F;
class TH3F;
class FitMaster;

class AlgoFit : public Algo {

 public:
  AlgoFit(FitMaster* fm);
  ~AlgoFit();

  void SetOption(std::string opt,bool val);
  Double_t GetChi2(std::vector<std::string> par_name, std::vector<Double_t> chi2_point, std::vector<Double_t> matrix_point);
 
  void Run();
  void RunFake(); // run fake data
  void Loop(Int_t n);

 private:
  bool fDoingRaster;

  bool fDoRaster;
  bool fDumpHistograms;
  bool fIs3D;

  Int_t fIter;
  TH2F* fHchi2;
  TH2F* fHchi2raster;
  TH3F* fHchi23D;

  
  std::vector<Double_t> fParLastMatrixUpdate;
  std::vector<Double_t> fRasterBestPar;
  std::vector<Double_t> fRasterBestChi2;
  

};
#endif
