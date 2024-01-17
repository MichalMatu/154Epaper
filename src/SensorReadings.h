#ifndef SENSOR_READINGS_H
#define SENSOR_READINGS_H

#include <Arduino.h>
#include <Adafruit_AHTX0.h>

class SensorReadings
{
public:
    SensorReadings(Adafruit_AHTX0 &ahtObject); // Pass the Adafruit_AHTX0 object in the constructor
    ~SensorReadings();

    float getTemperature();
    float getHumidity();

private:
    Adafruit_AHTX0 &aht; // Reference to the Adafruit_AHTX0 object
    float temperature;
    float humidity;
};

#endif // SENSOR_READINGS_H
