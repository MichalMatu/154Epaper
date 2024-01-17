#define ENABLE_GxEPD2_GFX 0

// #include <GxEPD2_BW.h>
// #include <GxEPD2_3C.h>
// #include <Fonts/FreeMonoBold12pt7b.h>

#include <Adafruit_AHTX0.h>

#include "DisplayUpdater.h"

DisplayUpdater displayUpdater;

// ESP32 CS(SS)=5, SCL(SCK)=18, SDA(MOSI)=23, BUSY=15, RES(RST)=12, DC=14

GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(5, 14, 12, 15));

void setup()

{
  Serial.begin(115200);

  display.init(115200, true, 50, false);
}

void loop()
{

  float temperature = 0;
  float humidity = 0;
  displayUpdater.showPartialUpdate(10, 15, 70, 20, temperature);
  displayUpdater.showPartialUpdate(10, 40, 70, 20, humidity);

  delay(10000);
}