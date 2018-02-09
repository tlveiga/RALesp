#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBold12pt7b.h>

class DisplayOLED
{
private:
  Adafruit_SSD1306 *_display;
  void print2digits(int number);

  const char *months[12] = {"Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"};
  const char *week[8] = {"Inv", "Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};

public:
  DisplayOLED();
  ~DisplayOLED();

  void test();
  void reset();

  void displayTime();

  void loop();
};

#endif