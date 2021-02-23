#include "Algo.h"
#include "Util.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>

#include "TH2F.h"
#include "TH3F.h"

#include "FitMaster.h"
#include "MsgLog.h"

using namespace std;

Algo::Algo(FitMaster* fm)
{
  fName = "Unnamed Algo";
  fFitMaster = fm;
  fMinChi2 = 9999999.;
}

Algo::~Algo()
{
}

void Algo::AddParameter(std::string name, Int_t ngrid, Double_t min, Double_t max, bool logscale)
{
  for (unsigned int i = 0; i < fPar.size(); i++) {
    if ( fParName[i] == name ) {
      MsgWarning(fName + "::Failed adding parameter " + name);
      return;
    }
  }
  
  fParName.push_back    ( name     );
  fLogScale.push_back   ( logscale );
  fNbins.push_back      ( ngrid    );
  fBin.push_back        ( 0        );
  fParMin.push_back     ( min      );
  fParMax.push_back     ( max      );
  fPar.push_back        ( min      );
  fParBestFit.push_back ( min      );
  if(name=="Eres"){
    fParNull.push_back  ( max      );
  }else{
    fParNull.push_back    ( min      );
  }
}

void Algo::SetParameterNull(std::string name, Double_t null)
{
  unsigned i = 0;
  while ( name != fParName[i] && i < fParName.size()) {
    if ( fParName[i] == name ) fParNull[i] = null;
    i++;
  }
}

TH2F* Algo::Get2dHist(std::string name, std::string xparname, std::string yparname, bool isxlog, bool isylog)
{
  // For non-log scale, the bin center is rom fParMin to fParMax + dx, where dx = (fParMax-fParMin) / (N-1)
  Double_t *haxisx = 0;
  Double_t *haxisy = 0;
  Int_t nx = 0;
  Int_t ny = 0;

  for (unsigned int i=0; i<fParName.size(); i++) {
    Double_t dx;
    Double_t bins[fNbins[i]];
    if (xparname == fParName[i]) {
      if ( isxlog ) {
        dx = ( log10(fParMax[i]) - log10(fParMin[i]) ) / Double_t(fNbins[i]-1);
        for (int j=0;j<=fNbins[i]; j++) bins[j] = pow(10,log10(fParMin[i])+dx*j);      
      } else {
        dx = ( fParMax[i] - fParMin[i] ) / Double_t(fNbins[i]-1);
        for (int j=0;j<=fNbins[i]; j++) bins[j] = fParMin[i]+dx*j;      
      }
      nx = fNbins[i];
      haxisx=Util::GetHistogramAxis(fNbins[i], bins);
    } else if (yparname == fParName[i]) {
      if ( isylog ) { 
        dx = ( log10(fParMax[i]) - log10(fParMin[i]) ) / Double_t(fNbins[i]-1);
        for (int j=0;j<=fNbins[i]; j++) bins[j] = pow(10,log10(fParMin[i])+dx*j);      
      } else {
        dx = ( fParMax[i] - fParMin[i] ) / Double_t(fNbins[i]-1);
        for (int j=0;j<=fNbins[i]; j++) bins[j] = fParMin[i]+dx*j;      
      }

      ny = fNbins[i];
      haxisy=Util::GetHistogramAxis(fNbins[i], bins);
    }
  }
  TH2F* hist=new TH2F(name.c_str(),Form("%s:%s",xparname.c_str(),yparname.c_str()),nx, haxisx, ny, haxisy);
  return hist;
}


TH3F* Algo::Get3dHist(std::string name, std::string xparname, std::string yparname, std::string zparname, bool isxlog, bool isylog, bool iszlog)
{
  MsgDebug(2, MsgLog::Form("Algo::Get3dHist with %d parNames", fParName.size()));
  // For non-log scale, the bin center is rom fParMin to fParMax + dx, where dx = (fParMax-fParMin) / (N-1)
  Double_t *haxisx = 0;
  Double_t *haxisy = 0;
  Double_t *haxisz = 0;
  Int_t nx = 0;
  Int_t ny = 0;
  Int_t nz = 0;

  for (unsigned int i=0; i<fParName.size(); i++) {
    Double_t dx;
    Double_t bins[fNbins[i]];
    if (xparname == fParName[i]) {
      if ( isxlog ) {
        dx = ( log10(fParMax[i]) - log10(fParMin[i]) ) / Double_t(fNbins[i]-1);
        for (int j=0;j<=fNbins[i]; j++) bins[j] = pow(10,log10(fParMin[i])+dx*j);      
      } else {
        dx = ( fParMax[i] - fParMin[i] ) / Double_t(fNbins[i]-1);
        for (int j=0;j<=fNbins[i]; j++) bins[j] = fParMin[i]+dx*j;      
      }
      nx = fNbins[i];
      haxisx=Util::GetHistogramAxis(fNbins[i], bins);
    } else if (yparname == fParName[i]) {
      if ( isylog ) { 
        dx = ( log10(fParMax[i]) - log10(fParMin[i]) ) / Double_t(fNbins[i]-1);
        for (int j=0;j<=fNbins[i]; j++) bins[j] = pow(10,log10(fParMin[i])+dx*j);      
      } else {
        dx = ( fParMax[i] - fParMin[i] ) / Double_t(fNbins[i]-1);
        for (int j=0;j<=fNbins[i]; j++) bins[j] = fParMin[i]+dx*j;      
      }
      ny = fNbins[i];
      haxisy=Util::GetHistogramAxis(fNbins[i], bins);
    } else if (zparname == fParName[i]) {
      if ( iszlog ) { 
        dx = ( log10(fParMax[i]) - log10(fParMin[i]) ) / Double_t(fNbins[i]-1);
        for (int j=0;j<=fNbins[i]; j++) bins[j] = pow(10,log10(fParMin[i])+dx*j);      
      } else {
        dx = ( fParMax[i] - fParMin[i] ) / Double_t(fNbins[i]-1);
        for (int j=0;j<=fNbins[i]; j++) bins[j] = fParMin[i]+dx*j;      
      }
      nz = fNbins[i];
      haxisz=Util::GetHistogramAxis(fNbins[i], bins);
    }
  }
  TH3F* hist=new TH3F(name.c_str(),Form("%s:%s:%s",xparname.c_str(),yparname.c_str(), zparname.c_str()),nx, haxisx, ny, haxisy, nz, haxisz);
  return hist;
}

void Algo::Dump()
{
  ostringstream oss;
  oss<<"Algorithm: "<<fName<<endl;
  oss<<endl;
  oss<<setw(12) << "Parameter"
  <<setw(8)  << "N"
  <<setw(10) << setprecision(5) << "Min"
  <<setw(10) << setprecision(5) << "Max"
  <<setw(10) << setprecision(5) << "Best Fit"
  <<setw(10) << setprecision(5) << "Current" << endl;
  for (Int_t i=0;i<60;i++) oss<<"-";
  oss<<endl;

  unsigned int i=0;
  while ( i < fPar.size() ) {
   oss<<setw(12) << fParName[i]
   <<setw(8)  << fNbins[i]
   <<setw(10) << fParMin[i]
   <<setw(10) << fParMax[i]
   <<setw(10) << fParBestFit[i]
   <<setw(10) << fPar[i] << endl;
   i++;
 }
 oss<<endl;
 MsgInfo(oss.str());
 oss.str("");
}
