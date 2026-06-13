#include <UIRenderer.h>
#include <time.h>
#include <FontManager.h>
#include <WiFi.h>

const unsigned char wifiIcon[] PROGMEM =
    {
        0b00011000,
        0b00100100,
        0b01000010,
        0b00000000,
        0b00011000,
        0b00100100,
        0b00011000,
        0b00000000};

void RenderHeader(String title)
{
    display.drawFastHLine(0, headerHeight, display.width(), GxEPD_BLACK);
    display.setFont(GetCurrentFont(0));

    display.setCursor(10, headerHeight * 0.75);
    display.println(title);

    struct tm *timeinfo;

    if (WiFi.getMode()==WIFI_STA)
    {
        display.setCursor(display.width() - 80, headerHeight * 0.75);
        display.drawBitmap(display.width() - 80,
                           5,
                           wifiIcon,
                           8,
                           8,
                           GxEPD_BLACK);
    }

    display.setCursor(display.width() - 50, headerHeight * 0.75);
    display.println(time(NULL));
}