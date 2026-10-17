#pragma once

#include "UI/Screens/BaseScreen.hpp"
#include "UI/Layout/LibraryLayout.hpp"
#include "Domain/Settings/LibrarySettings.hpp"
#include "Interfaces/IStorage.hpp"

class LibraryScreen : public BaseScreen<LibraryLayout>
{
public:
    LibraryScreen(
        IStorage &storage,
        IDisplay &display,
        DeviceSettings &deviceSettings,
        LibrarySettings &libSettings);

    void Enter() override;

    void Exit() override;

    void Draw() override;

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

    std::string GetParentPath(const std::string &path);

    bool IsRootDirectory() const;

    void EnsureSelectionVisible();

    void DrawListView();
    void DrawGridView();

    void RebuildLayout();

    void LoadDirectory(const std::string &path);
    bool IsSupportedBook(const std::string &filename);

private:
    void DrawScrollbar();

private:
    IStorage &m_storage;
    std::string m_rootBookPath = "/Books";
    std::string m_currentPath = "/Books";

    LibrarySettings m_libSettings;

    std::vector<DirectoryEntry> m_entries;

    size_t m_selectedIndex = 0;
    size_t m_firstVisibleIndex = 0;
};