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

int menu = 0;
// Read Measurement
uint16_t co2 = 0;
float temperature = 0.0f;
float humidity = 0.0f;

void menuFunc(int menu)
{

  switch (menu)
  {
  case 0:
    displayUpdater.showPartialUpdate(10, 10, 110, 40, temperature, "C");
    displayUpdater.showPartialUpdate(10, 70, 110, 40, humidity, "%");
    displayUpdater.showPartialUpdate(10, 130, 110, 40, co2, "ppm");

    // display T & H & CO2 after value

    break;
  case 1:
    display.firstPage();
    do
    {

      // Clear the screen
      display.fillScreen(GxEPD_WHITE);

      // Draw a filled rectangle
      display.fillRect(10, 10, 50, 30, GxEPD_BLACK);

      // Draw a line
      display.drawLine(70, 10, 120, 40, GxEPD_BLACK);

      // Draw a circle
      display.drawCircle(180, 25, 15, GxEPD_BLACK);

      // Draw text
      display.setCursor(10, 80);
      display.print("Hello, e-paper!");

      // Draw a filled triangle
      int16_t x0 = 70, y0 = 80, x1 = 120, y1 = 110, x2 = 170, y2 = 80;
      display.drawTriangle(x0, y0, x1, y1, x2, y2, GxEPD_BLACK);

    } while (display.nextPage());
    break;

  default:
    break;
  }
}

unsigned long previousMillis = 0;
const long interval = 10000; // Set the interval to 10 seconds

void loop()
{

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval)
  {
    // Save the current time
    previousMillis = currentMillis;
    uint16_t error;
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

    menuFunc(menu);

    // Your code to execute every 10 seconds goes here
  }
}
