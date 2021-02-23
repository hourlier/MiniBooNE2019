#ifndef OSCWEIGHTCALC_H
#define OSCWEIGHTCALC_H

#include "WeightCalc.h"
#include <memory>
class OscWeightCalc : public WeightCalc {

 public :
  OscWeightCalc();
  ~OscWeightCalc();

  Double_t     GetWeight(Event &event);
  Double_t     GetWeight(Double_t L, Double_t E, int inno=-1);
  bool         ParametersChanged();
  std::shared_ptr<WeightCalc>  Clone();   
};

#endif
