#include <App.h>
#include <GxEPD2_BW.h>
#include <Pins.h>
#include <DeviceSettings.h>
#include <LittleFS.h>
#include <Library.h>
#include <RenderSystem.h>
#include <Reader.h>
#include <UIRenderer.h>
#include <WifiMenu.h>
#include <SPI.h>


Reader reader;

App appInstance;

WifiMenu wifiMenu;

SPIClass spi(FSPI);


GxEPD2_BW<GxEPD2_420_GDEY042T81, GxEPD2_420_GDEY042T81::HEIGHT>
    display(GxEPD2_420_GDEY042T81(Display_CS, Display_DC, Display_RST, Display_BUSY));

void App::Begin()
{

    delay(500);

    spi.begin(SPI_CLK, SPI_MISO, SPI_MOSI);
    Serial.println("Starting...");

    DeviceSettingsMenuInstance.Init();

    currentState = State_Library;
    display.init(115200, true, 2, false);

    display.setTextColor(GxEPD_BLACK);

    pinMode(Pin_Navigation_Buttons, INPUT_PULLUP);
    pinMode(Pin_Menu_Buttons, INPUT_PULLUP);

    // Enable wakeup from buttons
    esp_sleep_enable_gpio_wakeup();

    gpio_wakeup_enable((gpio_num_t)Pin_Navigation_Buttons, GPIO_INTR_LOW_LEVEL);
    gpio_wakeup_enable((gpio_num_t)Pin_Menu_Buttons, GPIO_INTR_LOW_LEVEL);

    digitalWrite(Display_CS, HIGH);
    pinMode(Display_CS, OUTPUT);

    digitalWrite(SD_CS, HIGH);
    pinMode(SD_CS, OUTPUT);

    library.ScanAllBooks();

    RequestRefresh(Refresh_Full);
    ProcessRender();
}

void App::RenderCurrentScreen()
{
    display.fillScreen(GxEPD_WHITE);
    switch (currentState)
    {
    case State_Library:
        RenderHeader("Library");
        library.Render();
        break;
    case State_Reading:
        RenderHeader("Reading");
        reader.Render();
        break;
    case State_DeviceSettings:
        Serial.println("Rendering Device Settings");
        RenderHeader("Device Settings");
        DeviceSettingsMenuInstance.Render();
        break;
    case State_Wifi:
        RenderHeader("Wifi");
        wifiMenu.Render();
        break;
    default:
        break;
    }
}

void App::HandleInput(InputAction action)
{
    switch (currentState)
    {
    case State_Library:
        switch (library.HandleInput(action))
        {
        case LibraryResult_OpenBook:
            reader.OpenBook(library.GetSelectedBook());
            currentState = State_Reading;
            break;
        case LibraryResult_OpenSettings:
            DeviceSettingsMenuInstance.Init();
            currentState = State_DeviceSettings;
            break;
        default:
            break;
        }
        break;
    case State_DeviceSettings:
        switch (DeviceSettingsMenuInstance.HandleInput(action))
        {
        case DeviceSettingsResult_BackToLibrary:
            library.ScanAllBooks();
            currentState = State_Library;
            break;
        case DeviceSettingsResult_WifiMenu:
            wifiMenu.Begin();
            currentState = State_Wifi;
            break;
        default:
            break;
        }
    case State_Reading:
        switch (reader.HandleInput(action))
        {
        case InputResult_Back:
            library.ScanAllBooks();
            currentState = State_Library;
            break;
        default:
            break;
        }
        break;
    case State_Wifi:
        switch (wifiMenu.HandleInput(action))
        {
        case InputResult_Back:
            DeviceSettingsMenuInstance.Init();
            currentState = State_DeviceSettings;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    RequestRefresh(Refresh_Full);
    ProcessRender();
}

void App::ReadButtonPresses()
{
    int navigationValue = analogRead(Pin_Navigation_Buttons);
    int menuValue = analogRead(Pin_Menu_Buttons);
    // Serial.printf("Navigation: %d, Menu: %d\n", navigationValue, menuValue);
    if (navigationValue < 3800)
    {
        if (navigationValue < ADC_Button_UP)
        {
            HandleInput(Action_UP);
        }
        else if (navigationValue < ADC_Button_DOWN)
        {
            HandleInput(Action_DOWN);
        }
        else if (navigationValue < ADC_Button_LEFT)
        {
            HandleInput(Action_LEFT);
        }
        else if (navigationValue < ADC_Button_RIGHT)
        {
            HandleInput(Action_RIGHT);
        }
    }
    else if (menuValue < 3800)
    {
        if (menuValue < ADC_Button_SELECT)
        {
            HandleInput(Action_SELECT);
        }
        else if (menuValue < ADC_Button_BACK)
        {
            HandleInput(Action_BACK);
        }
    }
}

void App::Update()
{
    ReadButtonPresses();
    if (Serial.available())
    {
        char c = Serial.read();
        switch (c)
        {
        case 'w':
            HandleInput(Action_UP);
            break;
        case 's':
            Serial.println("DOWN");
            HandleInput(Action_DOWN);
            break;
        case 'a':
            HandleInput(Action_LEFT);
            break;
        case 'd':
            HandleInput(Action_RIGHT);
            break;
        case 'e':
            HandleInput(Action_SELECT);
            break;
        case 'q':
            HandleInput(Action_BACK);
            break;
        default:
            break;
        }
    }
    delay(500);
}