#include "Core/App.hpp"
#include "Core/Config.hpp"
#include "Domain/Reader/TextDocument.hpp"

static const char *TAG = "App";

GxEPD2_BW<GxEPD2_420_GDEY042T81, GxEPD2_420_GDEY042T81::HEIGHT>
    display(GxEPD2_420_GDEY042T81(Config::Display_CS, Config::Display_DC, Config::Display_RST, Config::Display_BUSY));

App::App() : m_storage(), m_library(m_storage), m_display(), m_screenManager(),
             m_libraryScreen(m_library, m_display), m_document(), m_readerScreen(m_document, m_display)
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
    m_library.Scan();

    // m_screenManager.SetScreen(
    //     &m_libraryScreen);

    // m_screenManager.Draw();

    auto file =
        m_storage.Open(
            "/Test.txt");

    m_document.Open(move(file));

    m_screenManager.SetScreen(
        &m_readerScreen);

    m_screenManager.Draw();

    return true;
}

void App::Update()
{
}