#include <iostream>
#include <math.h>
#include <memory>
#include "Event.h"
#include "ExpWeightCalc.h"


using namespace std;

ExpWeightCalc::ExpWeightCalc()
{ 
  fName = "ExpWeightCalc";

  fIsOn = true;

  fParName.resize(2);
  fPar.resize(2);
  fParLastCall.resize(2);

  fParName[0] = "A"; 
  fIsScalePar[fParName[0]] = true;
  fParMap[fParName[0]] = 0.;

  fParName[1] = "n";        
  fIsScalePar[fParName[1]] = false;
  fParMap[fParName[1]] = 0.;
}

ExpWeightCalc::~ExpWeightCalc()
{
}

std::shared_ptr<WeightCalc> ExpWeightCalc::Clone()
{
  std::shared_ptr<ExpWeightCalc> new_wc = std::make_shared<ExpWeightCalc>();
  new_wc->SetName(fName);
  for (unsigned int i=0; i<fParName.size(); i++) {
    new_wc->SetParameter(fParName[i], fPar[i]);
  }
 
  return new_wc;
}
  
Double_t ExpWeightCalc::GetWeight(Double_t L, Double_t E, int inno){
  Double_t wgh = fPar[0]*pow(L/E, fPar[1]);
  return wgh;
}

Double_t ExpWeightCalc::GetWeight(Event &event)
{
  // calculate oscillation
  if ( !fIsOn ) return 1.;

  fParLastCall = fPar;
  const Double_t L = event.nuleng();
  const Double_t E = event.enugen();
  return this->GetWeight(L, E);
}

bool ExpWeightCalc::ParametersChanged()
{
  bool changed = false;
  if ( fParLastCall != fPar ) {
    changed = true;
  }
  return changed;
}
