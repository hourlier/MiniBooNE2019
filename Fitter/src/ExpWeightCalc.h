#ifndef EXPWEIGHTCALC_H
#define EXPWEIGHTCALC_H

#include "WeightCalc.h"
#include <memory>

class ExpWeightCalc : public WeightCalc {

 public :
  ExpWeightCalc();
  ~ExpWeightCalc();

  Double_t     GetWeight(Event &event);
  Double_t 		GetWeight(Double_t L, Double_t E, int inno =-1);
  bool         ParametersChanged();
  std::shared_ptr<WeightCalc>  Clone();   
};

#endif
