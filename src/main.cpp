#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include "DisplayUpdater.h"
#include "bsec.h"
#include <Wire.h>

// Helper function declarations
void checkIaqSensorStatus();
void errLeds();

// Create an object of the class Bsec
Bsec iaqSensor;
String output;

DisplayUpdater displayUpdater;

// Define BME68X sensor address
#define BME68X_I2C_ADDR_HIGH 0x77

// Define gas resistance normalization parameters
const float minResistance = 60000.0;  // Replace with your actual minimum resistance
const float maxResistance = 100000.0; // Replace with your actual maximum resistance

GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(5, 14, 12, 15));

void setup()
{
  Serial.begin(115200);
  display.init(115200, true, 50, false);

  Wire.begin();
  iaqSensor.begin(BME68X_I2C_ADDR_HIGH, Wire);

  checkIaqSensorStatus();

  bsec_virtual_sensor_t sensorList[] = {
      BSEC_OUTPUT_IAQ,
      BSEC_OUTPUT_STATIC_IAQ,
      BSEC_OUTPUT_CO2_EQUIVALENT,
      BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
      BSEC_OUTPUT_RAW_TEMPERATURE,
      BSEC_OUTPUT_RAW_PRESSURE,
      BSEC_OUTPUT_RAW_HUMIDITY,
      BSEC_OUTPUT_RAW_GAS,
      BSEC_OUTPUT_STABILIZATION_STATUS,
      BSEC_OUTPUT_RUN_IN_STATUS,
      BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
      BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
      BSEC_OUTPUT_GAS_PERCENTAGE};

  iaqSensor.updateSubscription(sensorList, sizeof(sensorList) / sizeof(sensorList[0]), BSEC_SAMPLE_RATE_LP);
  checkIaqSensorStatus();
}

float normalizeGasResistance(float gasResistance)
{
  gasResistance = constrain(gasResistance, minResistance, maxResistance);
  return map(gasResistance, minResistance, maxResistance, 100, 0);
}

void loop()
{
  float normalizedGasResistance = normalizeGasResistance(iaqSensor.gasResistance);

  if (iaqSensor.run())
  {
    displayUpdater.showPartialUpdate(10, 15, 70, 20, iaqSensor.temperature);
    displayUpdater.showPartialUpdate(10, 40, 70, 20, iaqSensor.humidity);
    displayUpdater.showPartialUpdate(10, 65, 70, 20, iaqSensor.pressure);
    displayUpdater.showPartialUpdate(10, 90, 70, 20, normalizedGasResistance);
    // gasresistance
    displayUpdater.showPartialUpdate(10, 115, 70, 20, iaqSensor.gasResistance);
  }
  else
  {
    checkIaqSensorStatus();
  }

  delay(5000);
}

// Helper function definitions
void checkIaqSensorStatus()
{
  if (iaqSensor.bsecStatus != BSEC_OK)
  {
    output = "BSEC error code : " + String(iaqSensor.bsecStatus);
    Serial.println(output);
    for (;;)
      errLeds(); /* Halt in case of failure */
  }

  if (iaqSensor.bme68xStatus != BME68X_OK)
  {
    output = "BME68X error code : " + String(iaqSensor.bme68xStatus);
    Serial.println(output);
    for (;;)
      errLeds(); /* Halt in case of failure */
  }
}

void errLeds()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}
