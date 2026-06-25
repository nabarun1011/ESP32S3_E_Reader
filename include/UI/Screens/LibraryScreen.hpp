#pragma once

#include "UI/Screens/BaseScreen.hpp"
#include "UI/Layout/LibraryLayout.hpp"
#include "Domain/Settings/LibrarySettings.hpp"
#include "Interfaces/IStorage.hpp"

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

    void Refresh(RefreshMode mode = RefreshMode::Partial) override;

    void RefreshSelection();

    ScreenCommand HandleButton(Button button) override;

    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();

    void SortEntries();

    ScreenCommand OpenSelectedEntry();

    // To check if entry is supported book or directory or hidden item
    bool IsHiddenEntry(const DirectoryEntry &entry);

    String GetParentPath(const String &path);

    bool IsRootDirectory() const;

    void EnsureSelectionVisible();

    void DrawListView();
    void DrawGridView();

    void RebuildLayout();

    void LoadDirectory(const String &path);
    bool IsSupportedBook(const String &filename);


private:
    void DrawHeader();
    void DrawScrollbar();

private:
    IStorage &m_storage;
    String m_rootBookPath = "/Books";
    String m_currentPath = "/Books";

    LibrarySettings m_libSettings;

    std::vector<DirectoryEntry> m_entries;

    LibraryLayout m_layout;

    size_t m_selectedIndex = 0;
    size_t m_firstVisibleIndex = 0;
};