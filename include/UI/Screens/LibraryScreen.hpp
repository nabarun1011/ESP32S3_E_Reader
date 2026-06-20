#pragma once

#include "UI/Screens/BaseScreen.hpp"
#include "Domain/Library/LibraryScanner.hpp"
#include "Domain/Library/LibraryLayout.hpp"
#include "Domain/Settings/LibrarySettings.hpp"

class LibraryScreen : public BaseScreen
{
public:
    LibraryScreen(
        IStorage &storage,
        IDisplay &display,
        DeviceSettings &deviceSettings,
        LibrarySettings &libSettings);

    void Enter() override;

    void Exit() override;

    void Refresh() override;

    void HandleButton(Button button) override;

    bool HasBookOpenRequest() const;

    BookInfo TakeBookOpenRequest();

    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();

    void SortEntries();

    void OpenSelectedBook();

    void EnsureSelectionVisible();

    void DrawListView();
    void DrawGridView();

    void RebuildLayout();

    void LoadDirectory(const String &path);
    bool IsSupportedBook(const String &filename);

private:
    String m_rootBookPath = "/Books";
    String m_currentPath = "/Books";

    std::vector<DirectoryEntry> m_entries;

    LibrarySettings m_libSettings;

    IStorage &m_storage;

    LibraryLayout m_layout;

    size_t m_selectedIndex = 0;
    size_t m_firstVisibleIndex = 0;

    bool m_hasBookOpenRequest = false;

    BookInfo m_requestedBook;
};