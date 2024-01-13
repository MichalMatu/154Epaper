
// base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold12pt7b.h>

// ESP32 CS(SS)=5,SCL(SCK)=18,SDA(MOSI)=23,BUSY=15,RES(RST)=2,DC=0

// 1.54'' EPD Module
GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(/*CS=5*/ 5, /*DC=*/14, /*RES=*/12, /*BUSY=*/15)); // GDEH0154D67 200x200, SSD1681

void showPartialUpdate()
{
  uint16_t box_x = 10, box_y = 15, box_w = 70, box_h = 20;
  uint16_t cursor_y = box_y + box_h - 6;
  float value = 13.95;
  uint16_t incr = 1;

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

void setup()
{
  display.init(115200, true, 50, false);
  if (display.epd2.hasFastPartialUpdate)
  {
    showPartialUpdate();
    delay(1000);
  }
  display.hibernate();
}

void loop()
{
  // put your main code here, to run repeatedly:
}