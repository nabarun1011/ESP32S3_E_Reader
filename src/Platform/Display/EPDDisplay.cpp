#include "Platform/Display/EPDDisplay.hpp"
#include "Core/Config.hpp"
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeMono9pt7b.h>

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

int EPDDisplay::LineHeight(ReaderFont font) const
{
    return GetFont(font)->yAdvance;
}

int EPDDisplay::MeasureTextWidth(
    const String &text,
    ReaderFont font)
{
    m_display.setFont(GetFont(font));

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
    const String &text,
    ReaderFont font)
{
    m_display.setFont(GetFont(font));

    m_display.setCursor(
        x,
        y);

    m_display.print(text);
}

void EPDDisplay::DrawRect(
    int x,
    int y,
    int width,
    int height)
{
    m_display.drawRect(
        x,
        y,
        width,
        height,
        GxEPD_BLACK);
}

void EPDDisplay::FillRect(
    int x,
    int y,
    int width,
    int height)
{
    m_display.fillRect(
        x,
        y,
        width,
        height,
        GxEPD_WHITE);
}

void EPDDisplay::DrawLine(
    int x1,
    int y1,
    int x2,
    int y2)
{
    m_display.drawLine(
        x1,
        y1,
        x2,
        y2,
        GxEPD_BLACK);
}

const GFXfont *EPDDisplay::GetFont(
    ReaderFont font) const
{
    switch (font)
    {
    case ReaderFont::Serif:
        return &FreeSerif9pt7b;

    case ReaderFont::Sans:
        return &FreeSans9pt7b;

    case ReaderFont::Mono:
        return &FreeMono9pt7b;
    }

    return &FreeSans9pt7b;
}