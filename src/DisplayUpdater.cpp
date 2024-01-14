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

void DisplayUpdater::showPartialUpdate()
{
    const uint16_t box_x = 10, box_y = 15, box_w = 70, box_h = 20;
    const uint16_t cursor_y = box_y + box_h - 6;
    const float value = 13.95;
    const uint16_t incr = 1;

    display.setFont(&FreeMonoBold12pt7b);

    if (display.epd2.WIDTH < 104)
    {
        display.setFont(0);
    }

    display.setTextColor(GxEPD_BLACK);

    display.setRotation(3);
    display.setPartialWindow(box_x, box_y, box_w, box_h);

    // Perform a full update to clear the display initially
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
    } while (display.nextPage());

    for (uint16_t i = 1; i <= 10; i += incr)
    {
        display.firstPage();
        do
        {
            display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
            display.setCursor(box_x, cursor_y);
            display.print(value * i, 1);
        } while (display.nextPage());
        delay(1000);
    }
}
