#pragma once

#include "Interfaces/IDisplay.hpp"
#include "Domain/Reader/TextDocument.hpp"
#include "Interfaces/IScreen.hpp"
#include "Domain/Reader/TextRenderer.hpp"
#include "Domain/Settings/BookSettings.hpp"
#include "Domain/Settings/DeviceSettings.hpp"

class ReaderScreen : public IScreen
{
public:
    ReaderScreen(
        TextDocument &document,
        IDisplay &display,
        DeviceSettings &deviceSettings);

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Draw() override;

    void ApplyLayoutSettings();
    void RebuildPageIndex();

    void NextPage();
    void PreviousPage();
    void GoToPage(size_t page);

    void ToggleOverlayMenu();

    void MenuSelect();

    void MenuUp();
    void MenuDown();
    void MenuLeft();
    void MenuRight();

private:
    enum class State
    {
        Reading,
        Menu
    };

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
    struct ReaderSettings
    {
        String Font = "Default";

        int FontSize = 16;

        size_t CurrentPage = 0;

        size_t GoToPage = 0;

        size_t SelectedMenuItem = 0;
    };

    void DrawOverlayMenu();

private:
    TextDocument &m_document;
    IDisplay &m_display;
    String m_text;

    TextRenderer m_renderer;

    std::vector<size_t> m_pageStarts;

    State m_state = State::Reading;

    std::vector<String> m_menuItems =
        {
            "Resume",
            "Go To Page",
            "Library"};

    BookSettings m_bookSettings;
    ReaderSettings m_readerSettings;
    DeviceSettings &m_deviceSettings;
};