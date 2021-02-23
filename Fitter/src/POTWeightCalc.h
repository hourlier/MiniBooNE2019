#ifndef POTWEIGHTCALC_H
#define POTWEIGHTCALC_H

#include "WeightCalc.h"
#include <memory>

class POTWeightCalc : public WeightCalc {

 public :
  POTWeightCalc();
  ~POTWeightCalc();

  Double_t    GetWeight(Event &event);
  Double_t    GetWeight(Double_t L, Double_t E, int inno=-1);
  bool        ParametersChanged();
  std::shared_ptr<WeightCalc> Clone();

};

#endif
