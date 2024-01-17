#ifndef SCd41Reader_h
#define SCd41Reader_h

#include <Arduino.h>
#include <SensirionI2CScd4x.h>

class Scd41Reader
{
public:
    Scd41Reader();
    ~Scd41Reader();

    void begin();
    void readSensorValues(uint16_t &co2, float &temperature, float &humidity);

private:
    SensirionI2CScd4x scd4x;
};

#endif // SCd41Reader_h
