#include <Wire.h>

#include "src/mem.h"
#include "src/alarm.h"
#include "src/serverapi.h"
#include "src/displayoled.h"
#include "src/rtcmodule.h"

void alarmcb(AlarmStruct alarm);

const int nalarms = 10;

MemAccess *memaccess;
Alarm *alarms;
ServerConfig config;
ServerAPI *server;
DisplayOLED *oled;
RTCModule *rtc;

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Wire.begin(4, 5);

  memaccess = new MemAccess;
  memaccess->readServerConfig(&config);
  server = new ServerAPI(config);
  alarms = new Alarm(alarmcb);
  AlarmRaw buf[nalarms];
  memaccess->readAlarms(buf, nalarms);
  for (int i = 0; i < nalarms; i++)
    alarms->addAlarm(buf[i]);

  oled = new DisplayOLED();
  oled->test();
  rtc = new RTCModule();

  return;
  /*
  Serial.println(sizeof(ServerConfig));

  memaccess = new MemAccess();
  strcpy(config.ssid, "Sagrado");
  strcpy(config.password, "PBA#2k10$2010");
  memaccess->writeServerConfig(config);

  memaccess->readServerConfig(config);
  

  Serial.print("SSID: ");
  Serial.println(config.ssid);

  Serial.print("Password: ");
  Serial.println(config.password);*/

  // Serial.print("SSID: ");
  // Serial.println(config.ssid);

  // Serial.print("Password: ");
  // Serial.println(config.password);

  //EEPROM.end();
}

unsigned long lastupdate = 0;
void loop()
{
  alarms->loop();
  server->loop();
  memaccess->loop();
  oled->loop();
  rtc->loop();

  unsigned long n = millis();
  if (n - lastupdate > 1000)
  {
    lastupdate = n;
    oled->displayTime();
  }
}

void alarmcb(AlarmStruct alarm)
{
  Serial.println("Alarm fired");
}