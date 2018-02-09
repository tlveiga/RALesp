#include "rtcmodule.h"

#define UPTIMETIMESECONDS 3600

RTCModule::RTCModule()
{
    _rtc = new RtcDS1307<TwoWire>(Wire);

    if (_rtc->IsDateTimeValid())
    {
        //  printStatus(0, 0);
    }
    else
    {
        //   printStatus(1, 0);
        if (_rtc->GetIsRunning())
            _rtc->SetIsRunning(true);

        RtcDateTime Now = _rtc->GetDateTime();
        _lastupdate = Now.Epoch32Time();
        setTime(_lastupdate);
    }
}

RTCModule::~RTCModule()
{
    delete _rtc;
}

void RTCModule::loop()
{
    int n = _rtc->GetDateTime().Epoch32Time();
    if (n - _lastupdate > UPTIMETIMESECONDS) 
    {
        _lastupdate = n;
        setTime(_lastupdate);
    }
}