#include <iostream>
#include <math.h>
#include <memory>
#include "Event.h"
#include "ResExtraDimWeightCalc.h"
#include "MsgLog.h"


using namespace std;

ResExtraDimWeightCalc::ResExtraDimWeightCalc()
{ 
  fName = "ResExtraDimWeightCalc";

  fIsOn = true;

  fParName.resize(3);
  fPar.resize(3);
  fParLastCall.resize(3);

  fParName[0]              = "sin22theta"; 
  fIsScalePar[fParName[0]] = false;
  fParMap[fParName[0]]     = 0.;
  
  fParName[1]              = "dm2";        
  fIsScalePar[fParName[1]] = false;
  fParMap[fParName[1]]     = 0.;
  
  fParName[2]              = "Eres";        
  fIsScalePar[fParName[2]] = false;
  fParMap[fParName[2]]     = 1.e6;
  fPar[0] = 0;
  fPar[1] = 0;
  fPar[2] = 1.e6;

}

ResExtraDimWeightCalc::~ResExtraDimWeightCalc()
{
}

std::shared_ptr<WeightCalc> ResExtraDimWeightCalc::Clone()
{
  std::shared_ptr<ResExtraDimWeightCalc> new_wc = std::make_shared<ResExtraDimWeightCalc>();
  new_wc->SetName(fName);
  for (unsigned int i=0; i<fParName.size(); i++) {
    new_wc->SetParameter(fParName[i], fPar[i]);
  }
 
  return new_wc;
}

Double_t ResExtraDimWeightCalc::GetWeight(Event& event){
  const Double_t L = event.nuleng();
  const Double_t E = event.enugen();
  return this->GetWeight(L,E);
}

Double_t ResExtraDimWeightCalc::GetWeight(Double_t L, Double_t E, int inno)
{
  // calculate oscillation
  if ( !fIsOn ) return 1.;

  fParLastCall = fPar;


  const Double_t sinSq2theta = fPar[0];
  const Double_t dm2         = fPar[1];
  const Double_t Eres        = fPar[2];

  const Double_t cos2th       = sqrt(1. - sinSq2theta);
  const Double_t A            = cos2th * E * E / Eres / Eres;    // this part is the change
  const Double_t CSq          = pow(cos2th-A ,2) + sinSq2theta;
  const Double_t dm2m         = sqrt(CSq) * dm2;
  const Double_t sinSq2thetaM = sinSq2theta / CSq;

  Double_t wgh = sinSq2thetaM * pow(sin(1.267*dm2m * L / E), 2);
  //MsgDebug(4, MsgLog::Form("sinSq2theta=%.2f, dm2=%.2f, Eres=%.2f => prob = %.2e", sinSq2theta, dm2, Eres, wgh));
  return wgh;
}

bool ResExtraDimWeightCalc::ParametersChanged()
{
  bool changed = false;
  if ( fParLastCall != fPar ) {
    changed = true;
  }
  return changed;
}
