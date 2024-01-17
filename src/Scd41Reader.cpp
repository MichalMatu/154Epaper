#include "Scd41Reader.h"
#include <Wire.h>

Scd41Reader::Scd41Reader()
{
    // Constructor code, if any
}

Scd41Reader::~Scd41Reader()
{
    // Destructor code, if any
}

void Scd41Reader::begin()
{
    Wire.begin();
    scd4x.begin(Wire);
}

void Scd41Reader::readSensorValues(uint16_t &co2, float &temperature, float &humidity)
{
    uint16_t error;
    char errorMessage[256];

    // Read Measurement
    bool isDataReady = false;
    error = scd4x.getDataReadyFlag(isDataReady);
    if (error)
    {
        Serial.print("Error trying to execute getDataReadyFlag(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
        return;
    }

    if (!isDataReady)
    {
        return;
    }

    error = scd4x.readMeasurement(co2, temperature, humidity);
    if (error)
    {
        Serial.print("Error trying to execute readMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }
    else if (co2 == 0)
    {
        Serial.println("Invalid sample detected, skipping.");
    }
}
