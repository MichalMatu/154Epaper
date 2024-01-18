#include <Arduino.h>
#include <SensirionI2CScd4x.h>
#include <Wire.h>
#include "DisplayUpdater.h"

DisplayUpdater displayUpdater;
GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(5, 14, 12, 15));
SensirionI2CScd4x scd4x;

void printUint16Hex(uint16_t value)
{
  Serial.print(value < 4096 ? "0" : "");
  Serial.print(value < 256 ? "0" : "");
  Serial.print(value < 16 ? "0" : "");
  Serial.print(value, HEX);
}

void printSerialNumber(uint16_t serial0, uint16_t serial1, uint16_t serial2)
{
  Serial.print("Serial: 0x");
  printUint16Hex(serial0);
  printUint16Hex(serial1);
  printUint16Hex(serial2);
  Serial.println();
}

void readSensorValues(uint16_t &co2, float &temperature, float &humidity)
{
  delay(100);

  // Read Measurement
  bool isDataReady = false;
  uint16_t error = scd4x.getDataReadyFlag(isDataReady);
  if (error)
  {
    Serial.print("Error getting data ready flag: ");
    Serial.println(error);
    return;
  }

  if (!isDataReady)
  {
    return;
  }

  error = scd4x.readMeasurement(co2, temperature, humidity);
  if (error)
  {
    Serial.print("Error reading measurement: ");
    Serial.println(error);
  }
  else if (co2 == 0)
  {
    Serial.println("Invalid sample detected, skipping.");
  }
}

void setup()
{
  Serial.begin(115200);

  display.init(115200, true, 50, false);

  while (!Serial)
  {
    delay(100);
  }

  Wire.begin();

  uint16_t error;

  scd4x.begin(Wire);

  // stop potentially previously started measurement
  error = scd4x.stopPeriodicMeasurement();
  if (error)
  {
    Serial.print("Error stopping periodic measurement: ");
    Serial.println(error);
  }

  uint16_t serial0, serial1, serial2;
  error = scd4x.getSerialNumber(serial0, serial1, serial2);
  if (error)
  {
    Serial.print("Error getting serial number: ");
    Serial.println(error);
  }
  else
  {
    printSerialNumber(serial0, serial1, serial2);
  }

  // Start Measurement
  error = scd4x.startPeriodicMeasurement();
  if (error)
  {
    Serial.print("Error starting periodic measurement: ");
    Serial.println(error);
  }

  Serial.println("Waiting for first measurement... (5 sec)");
}

void loop()
{
  uint16_t error;

  delay(100);

  // Read Measurement
  uint16_t co2 = 0;
  float temperature = 0.0f;
  float humidity = 0.0f;
  readSensorValues(co2, temperature, humidity);

  if (co2 != 0)
  {
    Serial.print("Co2:");
    Serial.print(co2);
    Serial.print("\t");
    Serial.print("Temperature:");
    Serial.print(temperature);
    Serial.print("\t");
    Serial.print("Humidity:");
    Serial.println(humidity);
  }

  displayUpdater.showPartialUpdate(10, 10, 120, 40, temperature);
  displayUpdater.showPartialUpdate(10, 70, 120, 40, humidity);
  displayUpdater.showPartialUpdate(10, 130, 120, 40, co2);
  delay(10000);
}
