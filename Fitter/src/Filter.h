#ifndef FILTER_H
#define FILTER_H

#include <iostream>
#include <vector>
#include <string>
#include "Rtypes.h"

class Event;

class Filter {

 public:
  Filter();
  virtual ~Filter();

  bool Cut(Event& mb);
  void SetName(std::string name) {fName = name;};
  void SetCutParameter(std::string name) {fCutParName = name;}; 
  void SetCutParMin(Double_t val) {fCutParMin = val;};
  void SetCutParMax(Double_t val) {fCutParMax = val;};

  void Invert(bool b=true) { fInvert = b;};
  void Reset() { fEventCut = 0; fEventPass = 0;};
  void Dump();

 protected:
  std::string fName;

  std::string fCutParName;
  Double_t    fCutParMin;
  Double_t    fCutParMax;

  bool  fInvert;

  Int_t fEventCut;
  Int_t fEventPass;
};
#endif
