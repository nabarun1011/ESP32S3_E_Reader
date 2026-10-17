#pragma once

#include "Interfaces/IScreen.hpp"
#include "Domain/Settings/DeviceSettings.hpp"
#include "Interfaces/IDisplay.hpp"
#include "UI/Layout/ScreenLayout.hpp"
#include "UI/Components/Menu/Menu.hpp"

template <typename T_Layout>
class BaseScreen : public IScreen
{

protected:
    DeviceSettings &m_deviceSettings;
    IDisplay &m_display;
    T_Layout m_layout;

    void DrawLeftText(
        int x,
        int y,
        const std::string &text);

    void DrawRightText(
        int rightX,
        int y,
        const std::string &text);

    void DrawSelectionMarker(bool selected, int y);

    void DrawIndicator(
        MenuIndicator indicator,
        int rightX,
        int y,
        const std::string &text);

    void DrawAdjustableIndicator(int rightX, int y, const std::string &value);

    void DrawArrowIndicator(int rightX, int y);

    void UpdateLayout()
    {
        m_layout.HeaderHeight = m_display.LineHeight(m_deviceSettings.UIFont);
        m_layout.ContentY = m_layout.HeaderHeight + m_display.LineHeight(m_deviceSettings.UIFont) * 0.8;
        m_layout.FooterHeight = m_display.LineHeight(m_deviceSettings.UIFont);
        m_layout.ContentHeight = m_display.Height() - m_layout.FooterHeight - m_layout.HeaderHeight;
    }

public:
    BaseScreen(
        DeviceSettings &deviceSettings,
        IDisplay &display)
        : m_deviceSettings(deviceSettings),
          m_display(display)
    {
    }

    void DrawHeader(std::string text)
    {
        m_display.DrawLine(
            0,
            m_layout.HeaderHeight,
            m_display.Width(),
            m_layout.HeaderHeight);

        int headerY = m_display.LineHeight(m_deviceSettings.UIFont) * 0.8;

        m_display.DrawText(
            5,
            headerY,
            text,
            m_deviceSettings.UIFont);
    }

    void Refresh(const RefreshRequest &request) override
    {
        if (request.Full)
        {
            m_display.RenderFull(
                [this]()
                {
                    Draw();
                });
        }
        else
        {
            m_display.RenderPartial(
                request.x,
                request.y,
                request.width,
                request.height,
                [this]()
                {
                    Draw();
                });
        }
    }

    void Refresh(bool full = false) override
    {
        if (full)
        {
            m_display.RenderFull(
                [this]()
                {
                    Draw();
                });
        }
        else
        {
            m_display.RenderPartial(
                0,
                0,
                m_display.Width(),
                m_display.Height(),
                [this]()
                {
                    Draw();
                });
        }
    }
};

template <typename LayoutT>
void BaseScreen<LayoutT>::DrawLeftText(
    int x,
    int y,
    const std::string &text)
{
    m_display.DrawText(
        x,
        y,
        text,
        m_deviceSettings.UIFont);
}

template <typename LayoutT>
void BaseScreen<LayoutT>::DrawRightText(
    int rightX,
    int y,
    const std::string &text)
{
    m_display.DrawTextRight(
        rightX,
        y,
        text,
        m_deviceSettings.UIFont);
}

template <typename LayoutT>
void BaseScreen<LayoutT>::DrawSelectionMarker(
    bool selected,
    int y)
{
    if (!selected)
        return;

    DrawLeftText(
        5,
        y,
        ">");
}

template <typename LayoutT>
void BaseScreen<LayoutT>::DrawAdjustableIndicator(
    int rightX,
    int y,
    const std::string &value)
{
    DrawRightText(
        rightX,
        y,
        "< " + value + " >");
}

template <typename LayoutT>
void BaseScreen<LayoutT>::DrawArrowIndicator(
    int rightX,
    int y)
{
    DrawRightText(
        rightX,
        y,
        ">");
}
