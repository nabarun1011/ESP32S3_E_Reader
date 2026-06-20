#pragma once

#include "Domain/Reader/TextDocument.hpp"
#include "UI/Screens/BaseScreen.hpp"
#include "Domain/Reader/TextRenderer.hpp"
#include "Domain/Settings/BookSettings.hpp"
#include "Domain/Settings/ReaderSettings.hpp"
#include "Domain/Library/BookInfo.hpp"
#include "Interfaces/IBookSettingsRepository.hpp"
#include "Interfaces/IStorage.hpp"

class ReaderScreen : public BaseScreen
{
public:
    ReaderScreen(
        TextDocument &document,
        IDisplay &display,
        DeviceSettings &deviceSettings,
        IBookSettingsRepository &bookSettingsRepository);

    void Enter() override;
    void Exit() override;
    void HandleButton(Button button) override;
    void Refresh() override;

    void OpenBook(const BookInfo &book);

    void UpdateLayout();
    void ApplyReaderSettings();
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
    struct ReaderLayout
    {
        int TextTop = 5;
        int TextAreaHeight;

        int FooterY;
        int FooterHeight = 20;
    };
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

    void DrawOverlayMenu();

private:
    TextDocument &m_document;
    IBookSettingsRepository &m_bookSettingsRepository;
    BookInfo m_currentBook;
    String m_text;

    TextRenderer m_renderer;

    std::vector<size_t> m_pageStarts;

    State m_state = State::Reading;

    ReaderLayout m_layout;
    BookSettings m_bookSettings;
    ReaderSettings m_readerSettings;
};