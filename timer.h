//File Security:
#ifndef timer_h
#define timer_h

//----- Includes ---------------------------------------------------------------------------------------
//Local Headers:
//Arduino Librarys:
#include <Arduino.h>
//------------------------------------------------------------------------------------------------------


class Timer_Class {
public:
  void SET() { start_time = millis(); };
  unsigned long GET_TIME() { unsigned long time_passed = millis()-start_time; return time_passed;};
private:
  unsigned long start_time;
};

#endif
