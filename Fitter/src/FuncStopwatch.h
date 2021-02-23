#ifndef FUNCSTOPWATCH_H
#define FUNCSTOPWATCH_H
/*------------------------------------------------------

  FuncStopwatch


  Author: J.L. Klay (CalPoly)                           
  Date: 29-Apr-2008

-------------------------------------------------------*/
// Debug level:
// 1: Don't use it. Only use it when you want to take a look at something
// 2: Location indicator. Which function I am in
// 3: some details
// 4: full details


#include <string>
#include <iostream>
#include <memory>
#include <cstdio>
#include <map>
#include <chrono>
#include <string>


typedef std::chrono::high_resolution_clock::time_point TimeVar;
#define timeNow() std::chrono::high_resolution_clock::now()
#define duration(a) std::chrono::duration_cast<std::chrono::microseconds>(a).count()

class FuncStopwatch {

public:
  FuncStopwatch();  //default constructor
  ~FuncStopwatch();  //destructor

  static FuncStopwatch* Instance() { return fgInstance; }
  static void Start(const std::string itemName);
  static void Stop(const std::string itemName);
  static void Report();
  static void EnableStopwatch(bool enabled);

private:
  static FuncStopwatch* fgInstance;      //pointer to current instance

  std::map<std::string, int>     counter;    // count many entries 
  std::map<std::string, double>  totalTime;  // record the total time
  std::map<std::string, TimeVar> startTime;  // record the total time
  std::map<std::string, bool > watchActive;  // record whether it's actively recording time
  static bool       fgTimerEnabled;          // flag for counting time or not
};


// Start and stop
#define StartWatch(itemName) {FuncStopwatch::Start(itemName);}
#define StopWatch(itemName) {FuncStopwatch::Stop(itemName);}


#endif //FUNCSTOPWATCH_H
