#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold12pt7b.h>

#include "DisplayUpdater.h"

DisplayUpdater displayUpdater;

// ESP32 CS(SS)=5, SCL(SCK)=18, SDA(MOSI)=23, BUSY=15, RES(RST)=12, DC=14

GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(5, 14, 12, 15));

// Function to be executed on core 0
void taskCore0(void *parameter)
{
  for (;;)
  {
    Serial.println("Task on Core 0");
    delay(1000);
  }
}

// Function to be executed on core 1
void taskCore1(void *parameter)
{
  for (;;)
  {
    Serial.println("Task on Core 1");
    delay(1000);
  }
}

void setup()
{
  display.init(115200, true, 50, false);
  displayUpdater.showPartialUpdate(10, 15, 70, 20, 13.95); // Example values
  displayUpdater.showPartialUpdate(10, 40, 70, 20, 13.95); // Example values
  display.hibernate();
}

void loop()
{
  // Main loop code here
}
