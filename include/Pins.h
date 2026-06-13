#pragma once

#include <GxEPD2_BW.h>

// All pin defined here
constexpr int SPI_MISO = 13;
constexpr int SPI_CLK = 12;
constexpr int SPI_MOSI = 11;

// SD
constexpr int SD_CS = 10;

// Display
constexpr int Display_CS = 9;
constexpr int Display_DC = 8;
constexpr int Display_RST = 7;
constexpr int Display_BUSY = 6;

// Button Pins
constexpr int Pin_Navigation_Buttons = 5;
constexpr int Pin_Menu_Buttons = 4;

// ADC values of buttons
// 1k ohm resistor
constexpr int ADC_Button_UP = 60;
// 2.2k ohm resistor
constexpr int ADC_Button_DOWN = 100;
// 4.7k ohm resistor
constexpr int ADC_Button_LEFT = 250;
// 10k ohm resistor
constexpr int ADC_Button_RIGHT = 350;

// 1k ohm resistor
constexpr int ADC_Button_SELECT = 100;
// 4.7k ohm resistor
constexpr int ADC_Button_BACK = 300;

// Battery
constexpr int Battery_ADC = 2;

// Display pins
extern GxEPD2_BW<GxEPD2_420_GDEY042T81, GxEPD2_420_GDEY042T81::HEIGHT>
    display;

extern SPIClass spiSD;
