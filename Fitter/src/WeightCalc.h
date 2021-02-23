#ifndef WEIGHTCALC_H
#define WEIGHTCALC_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Rtypes.h"
#include "Consts.h"

class Event;

class WeightCalc {

 public :
  //  WeightCalc();
  virtual ~WeightCalc();


  virtual Double_t             GetWeight(Event &event)                          = 0;
  virtual Double_t             GetWeight(Double_t L, Double_t E,int inno)    = 0;
  virtual bool                 ParametersChanged()                              = 0;
  virtual Double_t             GetRescalingFactor(std::map<std::string, Double_t > par_map);
  virtual Consts::EUpdateType  KindOfUpdate(std::map<std::string, Double_t> par_map);
  virtual bool                 HaveAnyScaleParameter();
  virtual std::shared_ptr<WeightCalc>          Clone()                                     = 0;


  virtual void                 SetName(std::string name) { fName = name;};
  virtual void                 SetParameter(std::string name, Double_t val);
  virtual Int_t                GetNParameters(){return fPar.size();};


  std::string                      GetName()   {return fName;};
  std::map<std::string, Double_t > GetParMap() {return fParMap;};

  virtual void                 TurnOn() {fIsOn = true;};
  virtual void                 TurnOff() {fIsOn = false;};

  void                         Dump();

 protected :
  std::string fName;

  bool fParametersChanged;
  bool fIsOn;
  std::vector<Double_t>            fPar;
  std::vector<std::string>         fParName;
  std::vector<Double_t>            fParLastCall;

  std::map<std::string, bool>      fIsScalePar;
  std::map<std::string, Double_t > fParMap;

};

#endif
