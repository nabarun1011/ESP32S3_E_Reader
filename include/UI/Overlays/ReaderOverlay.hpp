#pragma once

#include <functional>

#include "UI/Overlays/BaseOverlay.hpp"

#include "Interfaces/IDisplay.hpp"

#include "Domain/Settings/BookSettings.hpp"
#include "Domain/Settings/DeviceSettings.hpp"
#include "Domain/Settings/ReaderSettings.hpp"
#include "Domain/Reader/PageIndexedEntry.hpp"
#include "UI/Components/Menu/Menu.hpp"

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
    void DrawMenuRow(
        const MenuItem &item,
        bool selected,
        int y);

    void HandleButton(Button button) override;

    void Show() override;

private:
    void MenuLeft();
    void MenuRight();
    void MenuSelect();

    void BuildMenu();
    enum class OverlayMenuItem
    {
        FontSize,
        Font,
        Orientation,
        GoToPage,
        Library,
        Resume,

        Count
    };
    MenuItem GetMenuItem(OverlayMenuItem menuItem);
    void UpdateMenuValues();

private:
    IDisplay &m_display;

    DeviceSettings &m_deviceSettings;

    BookSettings &m_bookSettings;

    ReaderSettings &m_readerSettings;

    std::function<void()> m_updateLayout;

    std::function<void(size_t)> m_goToPage;


    std::vector<PageIndexedEntry> &m_pages;

    Menu m_menu;
};