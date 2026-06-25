#pragma once

#include <functional>

#include "UI/Overlays/BaseOverlay.hpp"

#include "Interfaces/IDisplay.hpp"

#include "Domain/Settings/BookSettings.hpp"
#include "Domain/Settings/DeviceSettings.hpp"
#include "Domain/Settings/ReaderSettings.hpp"
#include "Domain/Reader/PageIndexedEntry.hpp"

class ReaderOverlay : public BaseOverlay
{
public:
    ReaderOverlay(
        IDisplay &display,
        DeviceSettings &deviceSettings,
        BookSettings &bookSettings,
        ReaderSettings &readerSettings,
        std::function<void()> updateLayout,
        std::function<void(size_t)> goToPage,
        std::vector<PageIndexedEntry> &pages);

    void Draw() override;

    void HandleButton(Button button) override;

private:
    void MenuUp();
    void MenuDown();
    void MenuLeft();
    void MenuRight();
    void MenuSelect();

    enum class OverlayMenuItem
    {
        FontSize,
        Font,
        Rotation,
        GoToPage,
        Library,
        Resume,

        Count
    };

private:
    IDisplay &m_display;

    DeviceSettings &m_deviceSettings;

    BookSettings &m_bookSettings;

    ReaderSettings &m_readerSettings;

    std::function<void()> m_updateLayout;

    std::function<void(size_t)> m_goToPage;

    std::vector<PageIndexedEntry> &m_pages;
};