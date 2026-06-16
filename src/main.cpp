#include <Arduino.h>
#include "Core/App.hpp"

App app;

void setup()
{
    Serial.begin(115200);

    while (!Serial)
    {
        delay(10);
    };

    Serial.println("Starting...");

    app.Init();
}

void loop()
{
    // if (buttonPressTimer.Ready())
    // {
    //     ReadButtonPresses();
    // }
    app.Update();

    // esp_light_sleep_start(); // Enter light sleep mode until the next button press or serial input

    // ReadButtonPresses();
    // delay(50); // Debounce delay
}
