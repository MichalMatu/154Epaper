// DisplayUpdater.h

#ifndef DISPLAY_UPDATER_H
#define DISPLAY_UPDATER_H

#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold18pt7b.h>

class DisplayUpdater
{
public:
    DisplayUpdater();
    ~DisplayUpdater();

    void showPartialUpdate(uint16_t x, uint16_t y, uint16_t w, uint16_t h, float value, String unit);

private:
    GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display;
};

#endif // DISPLAY_UPDATER_H
