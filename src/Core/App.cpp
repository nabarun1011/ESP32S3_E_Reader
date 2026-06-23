#include "Core/App.hpp"
#include "Core/Config.hpp"
#include "Domain/Reader/TextDocument.hpp"

static const char *TAG = "App";

GxEPD2_BW<GxEPD2_420_GDEY042T81, GxEPD2_420_GDEY042T81::HEIGHT>
    display(GxEPD2_420_GDEY042T81(Config::Display_CS, Config::Display_DC, Config::Display_RST, Config::Display_BUSY));

App::App() : m_storage(), m_bookSettingsRepository(m_storage), m_display(), m_screenManager(), m_deviceSettings(), m_libSettings(),
             m_libraryScreen(m_storage, m_display, m_deviceSettings, m_libSettings), m_document(),
             m_readerScreen(m_storage, m_document, m_display, m_deviceSettings, m_bookSettingsRepository), m_inputManager(m_screenManager)
{
}

bool App::Init()
{
    delay(1000);

    Serial.printf("%s : Init\n", TAG);
    Serial.printf(
        "__cplusplus = %ld\n",
        __cplusplus);
    SPI.begin(Config::SPI_CLK, Config::SPI_MISO, Config::SPI_MOSI);

    if (m_storage.Init() == false)
    {
        Serial.printf("%s : Storage Init Failed", TAG);
        return false;
    }

    if (!m_display.Init())
    {
        Serial.printf("%s : Display Init Failed", TAG);
        return false;
    }

    m_display.SetRotation(m_deviceSettings.Orientation);

    m_screenManager.SetScreen(
        &m_libraryScreen);

    return true;
}

void App::Update()
{
    if (!m_inputManager.Update())
    {
        return;
    }

    Button button = m_inputManager.GetLastStableButton();

    auto command = m_screenManager.HandleButton(button);

    switch (command.Type)
    {
    case ScreenCommandType::OpenBook:
        Serial.printf("%s Got open book command: %s\n", TAG, command.Book.path.c_str());
        m_readerScreen.SetBook(command.Book);
        m_screenManager.SetScreen(&m_readerScreen);

        break;
    case ScreenCommandType::OpenLibrary:
        m_screenManager.SetScreen(&m_libraryScreen);
        break;

    default:
        break;
    }
}