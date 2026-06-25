#pragma once

#include "Domain/Reader/TextDocument.hpp"
#include "UI/Screens/BaseScreen.hpp"
#include "Domain/Reader/TextRenderer.hpp"
#include "Domain/Settings/BookSettings.hpp"
#include "Domain/Settings/ReaderSettings.hpp"
#include "Domain/Library/BookInfo.hpp"
#include "Interfaces/IBookSettingsRepository.hpp"
#include "Interfaces/IStorage.hpp"
#include "UI/Layout/ReaderLayout.hpp"

class ReaderScreen : public BaseScreen
{
public:
    ReaderScreen(
        IStorage &storage,
        TextDocument &document,
        IDisplay &display,
        DeviceSettings &deviceSettings,
        IBookSettingsRepository &bookSettingsRepository);

    void Enter() override;
    void Exit() override;
    ScreenCommand HandleButton(Button button) override;
    void Refresh(RefreshMode mode) override;

    void SetBook(const BookInfo &book);

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

    struct PageIndexedEntry
    {
        size_t startLine = 0;
        size_t characterOffset = 0;
    };

    void
    DrawOverlayMenu();
    void DrawFooter();
    void DrawHeader();

private:
    IStorage &m_storage;
    TextDocument &m_document;
    IBookSettingsRepository &m_bookSettingsRepository;
    BookInfo m_currentBook;
    String m_text;

    TextRenderer m_renderer;

    std::vector<PageIndexedEntry> m_pageStarts;
    size_t FindPageForOffset(size_t offset) const;
    std::vector<WrappedLine> m_wrappedLines;

    State m_state = State::Reading;

    ReaderLayout m_layout;
    BookSettings m_bookSettings;
    ReaderSettings m_readerSettings;
};