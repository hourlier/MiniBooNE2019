#include "WeightCalc.h"
#include <iostream>
#include <iomanip>
#include "MsgLog.h"
using namespace std;

/*
WeightCalc::WeightCalc()
{
}
*/
WeightCalc::~WeightCalc()
{
}

void WeightCalc::SetParameter(std::string name, Double_t val)
{
  bool found= false;
  
  for (unsigned int i=0;i<fParName.size(); i++) {
    if ( name == fParName[i] ) {
      fPar[i] = val;
      fParMap[name] = val;
      found = true;
    }
  }
  
  // ignore parameters that aren't recognized
  if ( !found && (fName!="POTWeightCalc")) {
	 MsgWarning("WeightCalc " + fName + " Parameter " + name + " not recognized!");
  }
  
}

Double_t WeightCalc::GetRescalingFactor(std::map<std::string, Double_t > par_map)
{
  //assumes all scale factors are multiplicative
  //if that is not the case you need new GetRescalingFactor function in your WeightCalc
  Double_t scale = 1.;
  std::map<std::string, Double_t >::const_iterator it=fParMap.begin();
  while ( it != fParMap.end() ) {
    std::map<std::string, Double_t>::const_iterator om = par_map.find(it->first);
    if ( fIsScalePar[it->first] && om != par_map.end() ) {
      scale *= it->second / om->second;
    }
    it++;
  }
  return scale;
}

Consts::EUpdateType WeightCalc::KindOfUpdate(std::map<std::string, Double_t> par_map)
{
  //returns the kind of update one needs to do
  //if nothing changed kNoUpdate, if only scale factors changed kScaleUpdate else kFullUpdate
  //if old parameter was 0 it assumes it can't just rescale. if that is not the case
  //you probably need new KindOfUpdate function in your WeightCalc

  std::map<std::string, Double_t >::const_iterator it=fParMap.begin();
  Consts::EUpdateType utype = Consts::kNoUpdate;
  while ( it!= fParMap.end() ) {
    std::map<std::string, Double_t>::const_iterator om = par_map.find(it->first);
    if ( om != par_map.end() ) {
      if ( om->second == 0. )
        utype = Consts::kFullUpdate;
      if ( om->second != it->second && !fIsScalePar[it->first] )
        utype = Consts::kFullUpdate;
      if ( utype == Consts::kNoUpdate && om->second != it->second && fIsScalePar[it->first] )
        utype = Consts::kScaleUpdate;
    } else {
      utype = Consts::kFullUpdate;
    }
    it++;
  }
  
  return utype;
}

bool WeightCalc::HaveAnyScaleParameter(){
  bool haveScalePar = false;
  for(auto const& isScalePar : fIsScalePar){
    if(isScalePar.second==true) haveScalePar = true;
  }
  return haveScalePar;
}


void WeightCalc::Dump()
{
  MsgInfo("\n" + fName);
  for (unsigned int i=0;i<fPar.size();i++) {
    MsgInfo(MsgLog::Form("%20s%15.4f", fParName[i].c_str(), fPar[i]));
  }
}
