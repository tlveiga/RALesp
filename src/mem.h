#ifndef _MEM_H_
#define _MEM_H_
#include "alarm.h"

typedef struct ServerConfig
{
  char ssid[32];
  char password[32];
  char mqtt_url[32];
  char mqtt_id[32];
} ServerConfig;

class MemAccess
{
private:
  unsigned long _lastupdate;
  uint8_t _changed;

public:
  MemAccess();
  ~MemAccess();

  void update();
  void loop();

  void readServerConfig(ServerConfig *config);
  void writeServerConfig(const ServerConfig &config);

  void readAlarms(AlarmRaw *raw, size_t size);
  void writeAlarms(const AlarmRaw &raw, size_t size);

  void writeBlock(int address, uint8_t *buf, int size);
  void readBlock(int address, uint8_t *buf, int size);
};

#endif