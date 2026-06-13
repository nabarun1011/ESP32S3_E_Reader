#include <Arduino.h>
#include <Pins.h>
#include <LittleFS.h>
// #include <SD.h>
#include <SPI.h>
#include <vector>
#include <Preferences.h>
#include <InputActions.h>
#include <RenderSystem.h>
#include <App.h>
#include "driver/temp_sensor.h"
#include <SdFat.h>
#include <USBMSCManager.hpp>

struct Timer
{
    unsigned long lastTime = 0;
    unsigned long interval = 0;
    Timer(unsigned long intervalMs)
    {
        lastTime = 0;

        interval = intervalMs;
    }

    bool Ready()
    {
        if (millis() - lastTime > interval)
        {
            lastTime = millis();
            return true;
        }
        return false;
    }
};

Timer buttonPressTimer(500);

SdFat32 sd;
SPIClass spiSD(FSPI);
USBMSCManager usbmsc;



void setup()
{
    Serial.begin(115200);

    while (!Serial)
    {
        delay(10);
    }

    spiSD.begin(
        SPI_CLK,
        SPI_MISO,
        SPI_MOSI,
        SD_CS);

    SdSpiConfig sdConfig(
        SD_CS,
        SHARED_SPI,
        SD_SCK_MHZ(40),
        &spiSD);

    if (!sd.begin(sdConfig))
    {
        Serial.println("SD init failed");
        return;
    }
    Serial.printf("FAT type: %u\n", sd.fatType());
    Serial.printf("Cluster count: %u\n", sd.clusterCount());

        uint8_t sector[512];
    if (sd.card()->readSector(0, sector))
    {
        Serial.printf("Partition type: %02X\n", sector[450]);
    }
    if (sd.card()->readSector(0, sector))
    {
        Serial.printf(
            "Boot signature: %02X %02X\n",
            sector[510],
            sector[511]);

        Serial.printf(
            "Bytes/sector: %u\n",
            sector[11] | (sector[12] << 8));

        Serial.printf(
            "FAT type text: %.8s\n",
            &sector[82]);
    }
    Serial.println("Directory listing:");
    sd.ls(&Serial);

    // appInstance.Begin();

    // temp_sensor_config_t temp_sensor = TSENS_CONFIG_DEFAULT();

    // temp_sensor_set_config(temp_sensor);

    // temp_sensor_start();
}
void ReadButtonPresses()
{
    int navigationValue = analogRead(Pin_Navigation_Buttons);
    int menuValue = analogRead(Pin_Menu_Buttons);
    // Serial.printf("Navigation: %d, Menu: %d\n", navigationValue, menuValue);
    if (navigationValue < 3800)
    {
        if (navigationValue < ADC_Button_UP)
        {
            Serial.println("UP");
            usbmsc.Init(&sd);
        }
        else if (navigationValue < ADC_Button_DOWN)
        {
            Serial.println("DOWN");
        }
        else if (navigationValue < ADC_Button_LEFT)
        {
            Serial.println("LEFT");
        }
        else if (navigationValue < ADC_Button_RIGHT)
        {
            Serial.println("RIGHT");
        }
    }
    else if (menuValue < 3800)
    {
        if (menuValue < ADC_Button_SELECT)
        {
            Serial.println("SELECT");
        }
        else if (menuValue < ADC_Button_BACK)
        {
            Serial.println("BACK");
        }
    }
}

void loop()
{
    if (buttonPressTimer.Ready())
    {
        ReadButtonPresses();
    }

    // appInstance.Update();

    // esp_light_sleep_start(); // Enter light sleep mode until the next button press or serial input

    // ReadButtonPresses();
    // delay(50); // Debounce delay
}
