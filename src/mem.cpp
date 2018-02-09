#include "mem.h"

#include <EEPROM.h>
#include <Arduino.h>

const unsigned long meminit = 0x16585913;
#define MEMINITADDRESS 0
#define SERVERCONFIGADDRESS 4 + MEMINITADDRESS
#define ALARMSCONFIGADDRESS 128 + SERVERCONFIGADDRESS

#define UPDATETIME 60000

MemAccess::MemAccess()
{
  // tamanho 4 + 128 + (4 * 10)
  EEPROM.begin(512);
  unsigned long init;
  uint8_t valid = 1;
  for (int i = 0; i < 4 && valid; i++)
  {
    char c = EEPROM.read(MEMINITADDRESS + i);
    valid &= (c == ((meminit >> (i * 8)) & 0xFF));
  }
  if (valid == 0)
  {
    Serial.println("EEPROM reset.");
    for (int i = 0; i < 4; i++)
      EEPROM.write(MEMINITADDRESS + i, (meminit >> (i * 8)) & 0xFF);
    for (int i = SERVERCONFIGADDRESS; i < 512; i++)
      EEPROM.write(i, 0);

    EEPROM.commit();
  }
  else
  {
    Serial.println("EEPROM is good.");
  }

  _changed = 0;
}

MemAccess::~MemAccess()
{
  EEPROM.end();
}

void MemAccess::readServerConfig(ServerConfig *config)
{
  size_t size = sizeof(ServerConfig);
  uint8_t *memconfig = (uint8_t *)config;
  for (int i = 0; i < size; i++)
  {
    memconfig[i] = EEPROM.read(i + SERVERCONFIGADDRESS);
  }
}

void MemAccess::writeServerConfig(const ServerConfig &config)
{
  size_t size = sizeof(ServerConfig);
  const uint8_t *memconfig = (const uint8_t *)&config;
  for (int i = 0; i < size; i++)
  {
    EEPROM.write(i + SERVERCONFIGADDRESS, memconfig[i]);
  }
  update();
}

void MemAccess::readAlarms(AlarmRaw *raw, size_t size)
{
  uint8_t *ptr = (uint8_t *)raw;
  for (int i = 0; i < size; i++)
    *ptr++ = EEPROM.read(ALARMSCONFIGADDRESS + i);
}

void MemAccess::writeAlarms(const AlarmRaw &raw, size_t size)
{
  const uint8_t *ptr = (uint8_t *)&raw;
  for (int i = 0; i < size; i++)
    EEPROM.write(ALARMSCONFIGADDRESS + i, *ptr++);
  update();
}

void MemAccess::writeBlock(int address, uint8_t *buf, int size)
{
  for (int i = 0; i < size; i++)
    EEPROM.write(address + i, buf[i]);
  update();
}

void MemAccess::readBlock(int address, uint8_t *buf, int size)
{
  for (int i = 0; i < size; i++)
    buf[i] = EEPROM.read(address + i);
}

void MemAccess::update()
{
  _changed = 1;
  _lastupdate = millis();
}

void MemAccess::loop()
{
  if (_changed)
  {
    unsigned long now = millis();
    if (now - _lastupdate > UPDATETIME)
    {
      _changed = 0;
      EEPROM.commit();
      Serial.println("Committed to memory");
    }
  }
}