/*------------------------------------------------------
  FuncStopwatch

  NOTE: NEED TO MAKE THIS WORK ON MACOSX
  (The long comment output does not appear on the mac)

  Author: J.L. Klay (CalPoly)
  Date: 29-Apr-2008

-------------------------------------------------------*/

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <cstdarg>
#include <cstring>
#include <memory>

#include "FuncStopwatch.h"


FuncStopwatch* FuncStopwatch::fgInstance = 0x0;
bool FuncStopwatch::fgTimerEnabled = true;

//_____________________________________________________________________________
FuncStopwatch::FuncStopwatch()
{

  // replace the previous instance by this one
  if (fgInstance) delete fgInstance;
  fgInstance = this;

}

//_____________________________________________________________________________
FuncStopwatch::~FuncStopwatch() 
{
  //destructor: clean up and reset instance pointer

}


//_____________________________________________________________________________
void FuncStopwatch::EnableStopwatch(bool enabled)
{
  fgTimerEnabled = enabled;
}



//_____________________________________________________________________________
void FuncStopwatch::Report()
{
  // flush the output streams
  if(fgTimerEnabled==false) {
    std::cout << "timer was disabled" << std::endl;
  }
  std::cout << "Inside report" << std::endl;
  if (!fgInstance) {
    std::cout << "FuncWatch was never used!" << std::endl;
    return;
  }

  // make sorted total time by multimap
  std::multimap<double, std::string> sortedTotalTime;
  for(auto const& item : fgInstance->totalTime)
    sortedTotalTime.insert(make_pair(item.second, item.first));

  for(auto const& item : sortedTotalTime){
    printf("%-50s%10d%20.0f\n", item.second.c_str(), fgInstance->counter[item.second], item.first);
  }
}

//_____________________________________________________________________________
void FuncStopwatch::Start(const std::string itemName)
{
  // print a log message
  if(fgTimerEnabled==false) return;
  if (!fgInstance) new FuncStopwatch;

  fgInstance->watchActive[itemName] = true;
  fgInstance->counter[itemName]    += 1;
  fgInstance->startTime[itemName]   = timeNow();
}

void FuncStopwatch::Stop(const std::string itemName)
{
  // print a log message
  if(fgTimerEnabled==false) return;

  if (!fgInstance) {
    std::cout << "Error: FuncStopwatch was never started. Please do." << std::endl;
    return;
  }

  fgInstance->watchActive[itemName] = false;
  fgInstance->totalTime[itemName] += duration(timeNow() - fgInstance->startTime[itemName]);
}


