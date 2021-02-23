#include <iostream>
#include <math.h>
#include <memory>
#include "Event.h"
#include "OscWeightCalc.h"


using namespace std;

OscWeightCalc::OscWeightCalc()
{ 
  fName = "OscWeightCalc";

  fIsOn = true;

  fParName.resize(2);
  fPar.resize(2);
  fParLastCall.resize(2);

  fParName[0] = "sin22theta"; 
  fIsScalePar[fParName[0]] = true;
  fParMap[fParName[0]] = 0.;

  fParName[1] = "dm2";        
  fIsScalePar[fParName[1]] = false;
  fParMap[fParName[1]] = 0.;
}

OscWeightCalc::~OscWeightCalc()
{
}

std::shared_ptr<WeightCalc> OscWeightCalc::Clone()
{
  std::shared_ptr<OscWeightCalc> new_wc = std::make_shared<OscWeightCalc>();
  new_wc->SetName(fName);
  for (unsigned int i=0; i<fParName.size(); i++) {
    new_wc->SetParameter(fParName[i], fPar[i]);
  }
 
  return new_wc;
}

Double_t OscWeightCalc::GetWeight(Double_t L, Double_t E, int inno){
  Double_t wgh = fPar[0]*sin(1.27*fPar[1]*L/E)*sin(1.27*fPar[1]*L/E);
  return wgh;
}

Double_t OscWeightCalc::GetWeight(Event &event)
{
  // calculate oscillation
  if ( !fIsOn ) return 1.;

  fParLastCall = fPar;
  const Double_t L = event.nuleng();
  const Double_t E = event.enugen();
  
    return this->GetWeight(L, E);
}

bool OscWeightCalc::ParametersChanged()
{
  bool changed = false;
  if ( fParLastCall != fPar ) {
    changed = true;
  }
  return changed;
}
