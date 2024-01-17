#include <Arduino.h>
#include <Wire.h>
#include "DisplayUpdater.h"
#include "Scd41Reader.h"

DisplayUpdater displayUpdater;
Scd41Reader scd41Reader;

// ESP32 CS(SS)=5, SCL(SCK)=18, SDA(MOSI)=23, BUSY=15, RES(RST)=12, DC=14
GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(5, 14, 12, 15));

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  display.init(115200, true, 50, false);

  scd41Reader.begin();

  Serial.println("Waiting for first measurement... (5 sec)");
}

void loop()
{
  uint16_t co2 = 0;
  float temperature = 0.0f;
  float humidity = 0.0f;

  scd41Reader.readSensorValues(co2, temperature, humidity);

  Serial.print("Co2:");
  Serial.print(co2);
  Serial.print("\t");
  Serial.print("Temperature:");
  Serial.print(temperature);
  Serial.print("\t");
  Serial.print("Humidity:");
  Serial.println(humidity);

  displayUpdater.showPartialUpdate(10, 15, 70, 20, temperature);
  displayUpdater.showPartialUpdate(10, 40, 70, 20, humidity);
  displayUpdater.showPartialUpdate(10, 65, 70, 20, co2);

  delay(10000);
}
