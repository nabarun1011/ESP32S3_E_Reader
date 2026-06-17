#include "Platform/Display/EPDDisplay.hpp"
#include "Core/Config.hpp"

bool m_frameActive = false;

EPDDisplay::EPDDisplay() : m_display(
                               GxEPD2_420_GDEY042T81(
                                   Config::Display_CS,
                                   Config::Display_DC,
                                   Config::Display_RST,
                                   Config::Display_BUSY))
{
}

bool EPDDisplay::Init()
{
    m_display.init(115200, true, 2, false);
    m_display.setTextWrap(false);
    m_display.setRotation(0);
    m_display.setTextColor(GxEPD_BLACK);
    m_display.setTextSize(1);
    return true;
}

void EPDDisplay::Render(
    const std::function<void()> &drawFunc)
{
    m_display.setFullWindow();

    m_display.firstPage();

    do
    {
        m_display.fillScreen(
            GxEPD_WHITE);
        drawFunc();
    } while (m_display.nextPage());
}

int EPDDisplay::Width() const
{
    return m_display.width();
}

int EPDDisplay::Height() const
{
    return m_display.height();
}

int EPDDisplay::LineHeight() const
{
    return 16; // Temporary value
}

int EPDDisplay::MeasureTextWidth(
    const String &text)
{
    int16_t x1, y1;
    uint16_t w, h;
    m_display.getTextBounds(
        text,
        0,
        0,
        &x1,
        &y1,
        &w,
        &h);

    return w;
}

void EPDDisplay::SetTextSize(
    int size)
{
    m_display.setTextSize(
        size);
}

void EPDDisplay::SetRotation(
    uint8_t rotation)
{
    m_display.setRotation(rotation);
}

void EPDDisplay::DrawText(
    int x,
    int y,
    const String &text)
{
    m_display.setCursor(
        x,
        y);

    m_display.print(text);
}