#ifndef EVENT_H
#define EVENT_H

#include "Rtypes.h"

class Event
{
public:

  Event() {}
  ~Event() {}

  void iflux(Int_t val)    {fiflux  = val;};
  void ibkgd(Int_t val)    {fibkgd  = val;};
  void nuchan(Int_t val)   {fnuchan = val;};
  void inno(Int_t val)     {finno   = val;};
  void enugen(Double_t val) {fenugen = val;};
  void energy(Double_t val) {fenergy = val;};
  void nuleng(Double_t val) {fnuleng = val;};
  void parid(Int_t val)    {fparid  = val;};
  void wgt(Double_t val)    {fwgt    = val;};
  void ispi0(Int_t val)    {fispi0  = val;};
  void isdirt(Int_t val)   {fisdirt = val;};

  const Int_t    iflux()  {return fiflux;};
  const Int_t    ibkgd()  {return fibkgd;};
  const Int_t    nuchan() {return fnuchan;};
  const Int_t    inno()   {return finno;};
  const Double_t enugen() {return fenugen;};
  const Double_t energy() {return fenergy;};
  const Double_t nuleng() {return fnuleng;};
  const Int_t    parid()  {return fparid;};
  const Double_t wgt()    {return fwgt;};
  const Int_t    ispi0()  {return fispi0;};
  const Int_t    isdirt() {return fisdirt;};

 private:

  Int_t      fiflux;
  Int_t      fibkgd;
  Int_t      fnuchan;
  Int_t      finno;
  Double_t   fenugen;
  Double_t   fenergy;
  Double_t   fnuleng;
  Int_t      fparid;
  Double_t   fwgt;
  Int_t      fispi0;
  Int_t      fisdirt;

};
#endif
