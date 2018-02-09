#ifndef _RTCMODULE_H_
#define _RTCMODULE_H_
#include <Wire.h>
#include <TimeLib.h>
#include <RtcDS1307.h>

class RTCModule
{
  private:
    RtcDS1307<TwoWire> *_rtc;
    int _lastupdate;

  public:
    RTCModule();
    ~RTCModule();
    void loop();
};

#endif