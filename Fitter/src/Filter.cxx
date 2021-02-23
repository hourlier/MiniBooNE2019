#include "Filter.h"
#include <iostream>
#include <iomanip>
#include "MsgLog.h"
#include "Event.h"

using namespace std;

Filter::Filter()
{
  fName = "Unnamed filter";
  fInvert = false;
  Reset();
}

Filter::~Filter()
{
}

bool Filter::Cut(Event& mb)
{
  bool cut = true;
  Double_t dval = -9999.;
  Int_t    ival = -9999;
  if      ( fCutParName == "iflux"  ) ival = mb.iflux();
  else if ( fCutParName == "ibkgd"  ) ival = mb.ibkgd();
  else if ( fCutParName == "nuchan" ) ival = mb.nuchan();
  else if ( fCutParName == "inno"   ) ival = mb.inno();
  else if ( fCutParName == "enugen" ) dval = mb.enugen();
  else if ( fCutParName == "energy" ) dval = mb.energy();
  else if ( fCutParName == "nuleng" ) dval = mb.nuleng();
  else if ( fCutParName == "parid"  ) ival = mb.parid();
  else if ( fCutParName == "wgt"    ) dval = mb.wgt();
  else if ( fCutParName == "ispi0"  ) ival = mb.ispi0();
  else if ( fCutParName == "isdirt" ) ival = mb.isdirt();

  if      ( ival != -9999 && ival >= Int_t(fCutParMin) && ival <= Int_t(fCutParMax) ) cut = false;
  else if ( dval >= fCutParMin && dval <= fCutParMax ) cut = false;

  if (fInvert) cut = !cut;

  if ( cut )   fEventCut++;
  else         fEventPass++;

  return cut;
}
void Filter::Dump()
{
  MsgInfo(MsgLog::Form("%20s%20s", "Filter "         , fName.c_str()));
  MsgInfo(MsgLog::Form("%20s%20d", "Events passed "  , fEventPass ));
  MsgInfo(MsgLog::Form("%20s%20d", "Events cut "     , fEventCut  ));

}
