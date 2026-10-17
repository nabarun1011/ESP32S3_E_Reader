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
#include "UI/Overlays/ReaderOverlay.hpp"
#include "Domain/Reader/PageIndexedEntry.hpp"

class ReaderScreen : public BaseScreen<ReaderLayout>
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
    void Draw() override;

    void SetBook(const BookInfo &book);

    void ApplyReaderSettings();
    void RebuildPageIndex();

    void NextPage();
    void PreviousPage();
    void GoToPage(size_t page);

    void ToggleOverlayMenu();

private:
    void DrawOverlayMenu();
    void DrawFooter();

private:
    IStorage &m_storage;
    TextDocument &m_document;
    IBookSettingsRepository &m_bookSettingsRepository;

    BookInfo m_currentBook;
    std::string m_text;

    TextRenderer m_renderer;

    ReaderOverlay m_overlay;

    std::vector<PageIndexedEntry> m_pageStarts;
    size_t FindPageForOffset(size_t offset) const;
    std::vector<WrappedLine> m_wrappedLines;
    
    BookSettings m_bookSettings;
    ReaderSettings m_readerSettings;
};