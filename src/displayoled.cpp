#include "displayoled.h"
#include <TimeLib.h>

#define OLED_RESET 15
DisplayOLED::DisplayOLED()
{
  _display = new Adafruit_SSD1306(OLED_RESET);
  _display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
  reset();
}
DisplayOLED::~DisplayOLED()
{
  delete _display;
}

void DisplayOLED::loop()
{
}

void DisplayOLED::reset()
{
  _display->setTextSize(1);
  _display->setTextColor(WHITE);

  _display->clearDisplay();
  _display->setCursor(0, 0);
  _display->display();
}

void DisplayOLED::test()
{
  _display->setTextSize(3);
  _display->print("OLED");
  _display->display();
  delay(5000);
  reset();
}

void DisplayOLED::displayTime()
{
  _display->clearDisplay();
  _display->setTextSize(1);
  _display->setFont();
  _display->setTextColor(WHITE);
  _display->setCursor(0, 1);

  _display->print(week[weekday()]);
  _display->print(" ");
  _display->print(day());
  _display->print(' ');
  _display->print(months[month() - 1]);
  _display->print(' ');
  _display->print(year());

  _display->setCursor(15, 20);
  _display->setTextSize(1);
  _display->setFont(&FreeMonoBold12pt7b);
  _display->setTextColor(WHITE);

  print2digits(hour());
  _display->print(':');
  print2digits(minute());
  _display->print(':');
  print2digits(second());
  _display->println();

  _display->display();
}

void DisplayOLED::print2digits(int number) {
  if (number >= 0 && number < 10) {
    _display->print('0');
  }
  _display->print(number);
}