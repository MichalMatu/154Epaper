#include "SensorReadings.h"

SensorReadings::SensorReadings(Adafruit_AHTX0 &ahtObject) : aht(ahtObject)
{
    // Constructor code, if any
}

SensorReadings::~SensorReadings()
{
    // Destructor code, if any
}

float SensorReadings::getTemperature()
{
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp); // populate temp and humidity objects with fresh data
    return temp.temperature;        // Return the temperature value
}

float SensorReadings::getHumidity()
{
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);    // populate temp and humidity objects with fresh data
    return humidity.relative_humidity; // Return the humidity value
}
