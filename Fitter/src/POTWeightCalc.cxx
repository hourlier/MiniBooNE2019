#include "POTWeightCalc.h"
#include <iostream>
#include <memory>
#include "MsgLog.h"
using namespace std;

POTWeightCalc::POTWeightCalc()
{
  fName = "POTWeightCalc";

  fIsOn = true;

  fParName.resize(3);
  fPar.resize(3);
  fParLastCall.resize(3);

  fParName[0] = "POT"; 
  fParMap[fParName[0]] = 0.;
  fIsScalePar[fParName[0]] = true;

  fParName[1] = "n_mc_files"; 
  fParMap[fParName[1]] = 1.;
  fIsScalePar[fParName[1]] = true;

  fParName[2] = "pot_per_file"; 
  fParMap[fParName[2]] = 1.;
  fIsScalePar[fParName[2]] = true;
}

POTWeightCalc::~POTWeightCalc()
{
}

std::shared_ptr<WeightCalc> POTWeightCalc::Clone()
{
  std::shared_ptr<POTWeightCalc> new_wc = std::make_shared<POTWeightCalc>();
  new_wc->SetName(fName);
  for (unsigned int i=0; i<fParName.size(); i++) {
    new_wc->SetParameter(fParName[i], fPar[i]);
  }

  return new_wc;
}

Double_t POTWeightCalc::GetWeight(Event &event)
{
  fParLastCall = fPar;
  Double_t wgh = fPar[0]/(fPar[1]*fPar[2]);

  return wgh;
}
Double_t  POTWeightCalc::GetWeight(Double_t L, Double_t E, int inno){
  MsgFatal("POTWeightCalc shouldn't use GetWeight(L,E)!");
  return 0;
}


bool POTWeightCalc::ParametersChanged()
{
  bool changed = false;
  if ( fParLastCall != fPar )
    changed = true;
  
  return changed;
}
