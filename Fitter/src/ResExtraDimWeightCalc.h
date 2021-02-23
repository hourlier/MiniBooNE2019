#ifndef ResExtraDimWeightCalc_H
#define ResExtraDimWeightCalc_H

// This code is from the suggestion of Heinrich
//  https://arxiv.org/abs/hep-ph/0504096
// for extra dimnension


#include "WeightCalc.h"
#include <memory>

class ResExtraDimWeightCalc : public WeightCalc {

 public :
  ResExtraDimWeightCalc();
  ~ResExtraDimWeightCalc();

  Double_t     GetWeight(Event &event);
  Double_t     GetWeight(Double_t L, Double_t E, int inno=-1);
  bool         ParametersChanged();
  std::shared_ptr<WeightCalc>  Clone();   
};

#endif
