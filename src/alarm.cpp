#include "Alarm.h"
#include <Arduino.h>

#define NEWARRAYSIZE 10;

// MACROS
#define DOW(raw) ((uint8_t)((uint)(raw & 0x7f) << 1))
#define HOURS(raw) ((uint8_t)((raw >> 7) & 0x1f))
#define MINUTES(raw) ((uint8_t)((raw >> 12) & 0x3f))
#define ACTIVE(raw) ((uint8_t)((raw >> 18) & 0x1))
#define REPEAT(raw) ((uint8_t)((raw >> 19) & 0x1))
#define ACTION(raw) ((uint16_t)((raw >> 20) & 0xfff))
#define RAW(str) (((str.dow >> 1) & 0x7f) + ((str.hours & 0x1f) << 7) + ((str.minutes & 0x3f) << 12) + ((str.active & 0x1) << 18) + ((str.repeat & 0x1) << 19) + ((str.action & 0xfff) << 20))

Alarm::Alarm(const AlarmCallback callback)
{
  _callback = callback;
  _arraysize = NEWARRAYSIZE;
  _alarms = (AlarmRaw *)malloc(_arraysize * sizeof(AlarmRaw));
  _nalarms = 0;
  _lastcheckminute = -1;
}

Alarm::~Alarm()
{
  free(_alarms);
}

void Alarm::loop()
{
  int minutes = minute();
  if (minutes != _lastcheckminute)
  {
    _lastcheckminute = minutes;
    int dow = weekday();
    int hours = hour();
    for(int i = 0; i < _nalarms; i++) {
      AlarmStruct alm = getAlarm(i);
      if (alm.active && ((alm.dow >> dow) & 0x1) && alm.minutes == minutes && alm.hours == hours) {
        // Considerar se devo implementar o desactivar do alarme quando o repeat é 0, é preciso notificar o "mem" para guardar a alteração
        if (alm.repeat  == 0) {
          alm.active = 0;
          _alarms[i] ^= 1 << 18; // testar
        }
        _callback(alm);
      }
    }
  }
}

bool Alarm::addAlarm(const AlarmStruct &alarm)
{
  AlarmRaw raw = RAW(alarm);
  return addAlarm(raw);
}

bool Alarm::addAlarm(const AlarmRaw raw)
{
  if (_nalarms == _arraysize)
  {
    _arraysize += NEWARRAYSIZE;
    _alarms = (AlarmRaw *)realloc(_alarms, _arraysize * sizeof(AlarmRaw));
  }
  _alarms[_nalarms++] = raw;

  return true;
}

bool Alarm::updateAlarm(const AlarmStruct &alarm)
{
  if (alarm.id < 0 || alarm.id >= _nalarms)
  {
    return false;
  }
  else
  {
    _alarms[alarm.id] = RAW(alarm);
    return true;
  }
}

size_t Alarm::count()
{
  return _nalarms;
}

AlarmStruct Alarm::getAlarm(int alarmid)
{
  AlarmStruct retval;
  if (alarmid < 0 || alarmid >= _nalarms)
    retval.id = -1;
  else
  {
    AlarmRaw raw = _alarms[alarmid];
    retval.id = alarmid;
    retval.dow = DOW(raw);
    retval.hours = HOURS(raw);
    retval.minutes = MINUTES(raw);
    retval.repeat = REPEAT(raw);
    retval.active = ACTIVE(raw);
    retval.action = ACTION(raw);
  }

  return retval;
}

size_t Alarm::getRawData(AlarmRaw *buf, size_t size)
{
  size_t i;
  for (i = 0; i < size && i < _nalarms; i++)
    buf[i] = _alarms[i];
  return i;
}

/*
int Alarm::addAlarm(uint8_t dow, uint8_t hours, uint8_t minutes, uint8_t active, uint8_t repeat)
{
  AlarmStruct *alarm = new AlarmStruct;
  alarm->dow = dow;
  alarm->hours = hours;
  alarm->minutes = minutes;
  alarm->active = active;
  alarm->repeat = repeat;
  alarm->next = NULL;
  if (_alarms == NULL)
  {
    alarm->id = 1;
    _alarms = alarm;
  }
  else {
    AlarmStruct *ptr = _alarms;
    while (ptr->next != NULL) {
      ptr = ptr->next;
    }
    alarm->id = ptr->id + 1;
    ptr->next = alarm;
  }

  return alarm->id;
}
int Alarm::removeAlarm(int alarmid)
{
  AlarmStruct *ptr = _alarms, *prev = _alarms;
  while (ptr != NULL && ptr->id != alarmid)
  {
    prev = ptr;
    ptr = ptr->next;
  }

  if (ptr != NULL)
  {
    if (ptr == _alarms)
    {
      _alarms = ptr->next;
    }
    else
    {
      prev->next = ptr->next;
    }

    delete ptr;
    return 1;
  }
  else
  {
    return -1;
  }
}
int Alarm::updateAlarm(int alarmid, uint8_t dow, uint8_t hours, uint8_t minutes, uint8_t active, uint8_t repeat)
{
  AlarmStruct *alm = getAlarm(alarmid);
  if (alm != NULL)
  {
    alm->dow = dow;
    alm->hours = hours;
    alm->minutes = minutes;
    alm->repeat = repeat;
    return 1;
  }
  else
    return -1;
}

uint8_t Alarm::count()
{
  AlarmStruct *ptr = _alarms;
  uint8_t count = 0;
  while (ptr != NULL)
  {
    count++;
    ptr = ptr->next;
  }

  return count;
}
int Alarm::getID(uint8_t index)
{
  AlarmStruct *ptr = _alarms;
  int idx = 0;
  while (ptr != NULL && idx != index)
  {
    ptr = ptr->next;
    idx++;
  }
  return ptr != NULL ? ptr->id : -1;
}
int Alarm::getIndex(int alarmid)
{
  int index = -1;
  AlarmStruct *ptr = _alarms;
  while (ptr != NULL)
  {
    index++;
    if (ptr->id == alarmid)
    {
      break;
    }
    else
    {
      ptr = ptr->next;
    }
  }
  return index;
}
uint8_t Alarm::getDOW(int alarmid)
{
  AlarmStruct *alm = getAlarm(alarmid);
  return alm != NULL ? alm->dow : Alarm::ERR;
}
uint8_t Alarm::getHours(int alarmid)
{
  AlarmStruct *alm = getAlarm(alarmid);
  return alm != NULL ? alm->hours : Alarm::ERR;
}
uint8_t Alarm::getMinutes(int alarmid)
{
  AlarmStruct *alm = getAlarm(alarmid);
  return alm != NULL ? alm->minutes : Alarm::ERR;
}

AlarmStruct *Alarm::getAlarm(int alarmid)
{
  AlarmStruct *ptr = _alarms;
  while (ptr != NULL && ptr->id != alarmid)
  {
    ptr = ptr->next;
  }
  return ptr;
}
*/