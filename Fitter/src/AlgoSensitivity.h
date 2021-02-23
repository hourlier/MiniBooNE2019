#ifndef ALGOSENSITIVITY_H
#define ALGOSENSITIVITY_H

#include "Algo.h"

class TH2F;
class FitMaster;

class AlgoSensitivity : public Algo {

 public:
  AlgoSensitivity(FitMaster* fm);
  ~AlgoSensitivity();

  void Run();
  void Loop(Int_t n);

 private:
  TH2F* fHchi2;

};
#endif
