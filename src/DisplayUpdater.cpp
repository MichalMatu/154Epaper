// DisplayUpdater.cpp

#include "DisplayUpdater.h"

DisplayUpdater::DisplayUpdater() : display(GxEPD2_154_D67(5, 14, 12, 15))
{
    // Constructor code, if any
}

DisplayUpdater::~DisplayUpdater()
{
    // Destructor code, if any
}

void DisplayUpdater::showPartialUpdate(uint16_t box_x, uint16_t box_y, uint16_t box_w, uint16_t box_h, float value)
{
    const uint16_t cursor_y = box_y + box_h - 6;

    display.setFont(&FreeMonoBold24pt7b);

    display.setTextColor(GxEPD_BLACK);

    display.setRotation(3);
    display.setPartialWindow(box_x, box_y, box_w, box_h);

    // Perform a full update to clear the display initially
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
    } while (display.nextPage());

    display.firstPage();
    do
    {
        display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
        display.setCursor(box_x, cursor_y);
        display.print(value, 1);
    } while (display.nextPage());
    // delay(1000);
}
