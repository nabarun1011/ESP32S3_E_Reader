#include "UI/Screens/LibraryScreen.hpp"

const char *TAG = "LibraryScreen";

LibraryScreen::LibraryScreen(
    IStorage &storage, IDisplay &display,
    DeviceSettings &deviceSettings,
    LibrarySettings &libSettings)
    : BaseScreen(deviceSettings, display), m_storage(storage), m_libSettings(libSettings)
{
}

void LibraryScreen::Enter()
{
    Serial.printf("%s : LibraryScreen Selection index: %d\n", TAG, m_selectedIndex);
    LoadDirectory(m_rootBookPath);
}

void LibraryScreen::Exit()
{
    Serial.printf("%s : LibraryScreen Exit\n", TAG);
}
void LibraryScreen::RebuildLayout()
{
    m_layout.ContentHeight = m_display.Height() - m_layout.HeaderHeight - m_layout.FooterHeight;

    switch (m_libSettings.LibraryView)
    {
    case LibraryViewMode::List:

        m_layout.Columns = 1;

        // Height of each book entry in list view
        m_layout.ItemHeight = m_display.LineHeight(m_deviceSettings.UIFont);

        break;

    case LibraryViewMode::Grid:

        // placeholder

        m_layout.Columns = 2;

        m_layout.ItemHeight = 80;

        break;
    }
}

bool LibraryScreen::IsRootDirectory() const
{
    return m_currentPath == m_rootBookPath;
}

String LibraryScreen::GetParentPath(const String &path)
{
    // Find position of last "/"
    // Eg. /Books/Adevnture/AdventureStory.txt
    int pos = path.lastIndexOf('/');

    // If path is /Books
    //  "/" position will be 0
    // Then path is already root and also becomes root
    if (pos <= 0)
    {
        return m_rootBookPath;
    }

    // If path is /Books/Adventure/AdventureStory.txt
    // Returns /Books/Adventure
    return path.substring(0, pos);
}

void LibraryScreen::EnsureSelectionVisible()
{
    // Number of visible items
    size_t visibleItems = m_layout.VisibleItemsPerPage();

    // If selected entry is in previous page then move to previous page
    if (m_selectedIndex < m_firstVisibleIndex)
    {
        m_firstVisibleIndex = m_selectedIndex;
    }

    // If selected entry is in next page then move to next page
    if (m_selectedIndex >= m_firstVisibleIndex + visibleItems)
    {
        m_firstVisibleIndex = m_selectedIndex - visibleItems + 1;
    }
}

void LibraryScreen::Refresh(RefreshMode mode)
{
    RebuildLayout();
    switch (m_libSettings.LibraryView)
    {
    case LibraryViewMode::List:
        if (mode == RefreshMode::Full)
        {
            m_display.RenderFull([this]()
                                 { DrawHeader(); 
                                    DrawListView(); });
        }
        else
        {
            RefreshSelection();
        }
        // DrawScrollbar();
        break;
    case LibraryViewMode::Grid:
        DrawGridView();
        break;
    default:
        break;
    }
}
void LibraryScreen::RefreshSelection()
{
    int height = m_display.Height();

    m_display.RenderPartial(
        0,
        0,
        m_display.Width(),
        height,
        [this]()
        {
            DrawHeader();
            DrawListView();
        });
}

// void LibraryScreen::DrawScrollbar()
// {
//     if (m_entries.empty())
//     {
//         return;
//     }

//     if (m_entries.size() <= m_layout.VisibleRows)
//     {
//         return;
//     }

//     const int trackWidth = 4;

//     const int trackX =
//         m_display.Width() -
//         trackWidth -
//         2;

//     const int trackY =
//         m_layout.HeaderHeight;

//     const int trackHeight =
//         m_layout.VisibleRows *
//         m_layout.ItemHeight;

//     m_display.DrawRect(
//         trackX,
//         trackY,
//         trackWidth,
//         trackHeight);

//     int thumbHeight =
//         (trackHeight *
//          m_layout.VisibleRows) /
//         m_entries.size();

//     thumbHeight =
//         std::max(
//             8,
//             thumbHeight);

//     float progress =
//         static_cast<float>(
//             m_topIndex) /
//         static_cast<float>(
//             m_entries.size() -
//             m_layout.VisibleRows);

//     int thumbY =
//         trackY +
//         static_cast<int>(
//             progress *
//             (trackHeight -
//              thumbHeight));

//     m_display.FillRect(
//         trackX,
//         thumbY,
//         trackWidth,
//         thumbHeight);
// }

void LibraryScreen::LoadDirectory(const String &path)
{
    m_currentPath = path;

    m_entries = m_storage.ListDirectory(path);

    // Remove hidden entries
    m_entries.erase(
        std::remove_if(
            m_entries.begin(),
            m_entries.end(),
            [this](const DirectoryEntry &entry)
            {
                return IsHiddenEntry(
                    entry);
            }),
        m_entries.end());

    SortEntries();

    // If not root directory then add ".." entry as parent at top
    if (!IsRootDirectory())
    {
        DirectoryEntry parentDir;
        parentDir.name = "..";
        parentDir.path = GetParentPath(path);
        parentDir.isDirectory = true;
        parentDir.size = 0;
        m_entries.insert(m_entries.begin(), parentDir);
    }

    m_selectedIndex = 0;

    m_firstVisibleIndex = 0;

    Refresh(RefreshMode::Full);
}

void LibraryScreen::SortEntries()
{
    std::sort(
        m_entries.begin(), m_entries.end(), [this](const DirectoryEntry &a, const DirectoryEntry &b)
        {
            if (a.isDirectory != b.isDirectory)
            {
                return a.isDirectory;
            }

            String aName = a.name;
            String bName = b.name;

            aName.toLowerCase();
            bName.toLowerCase();           

            switch (m_libSettings.LibrarySorting)
            {
            case LibrarySort::NameAscending:
                return aName < bName;

            case LibrarySort::NameDescending:
                return aName > bName;
            case LibrarySort::DateNewestFirst:
                return a.modifiedTime >b.modifiedTime;

            case LibrarySort::DateOldestFirst:
                return a.modifiedTime <b.modifiedTime;

            case LibrarySort::SizeLargestFirst:
                return a.size >b.size;

            case LibrarySort::SizeSmallestFirst:
                return a.size <b.size;
            default:
                return aName < bName;
            } });
}

bool LibraryScreen::IsSupportedBook(const String &filename)
{
    String lower = filename;

    lower.toLowerCase();

    return lower.endsWith(".txt") ||
           lower.endsWith(".md") ||
           lower.endsWith(".epub");
}

bool LibraryScreen::IsHiddenEntry(
    const DirectoryEntry &entry)
{
    if (entry.isDirectory)
    {
        return false;
    }

    return entry.name.endsWith(
        ".settings");
}

void LibraryScreen::DrawHeader()
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
        "Library",
        m_deviceSettings.UIFont);
}

void LibraryScreen::DrawListView()
{

    int contentWidth = m_display.Width() - 4;
    const auto &books = m_entries;

    size_t visibleItems = m_layout.VisibleRows();

    size_t endIndex = std::min(
        books.size(),
        m_firstVisibleIndex +
            visibleItems);

    int y = m_layout.HeaderHeight + m_display.LineHeight(m_deviceSettings.UIFont);

    for (size_t i = m_firstVisibleIndex; i < endIndex; ++i)
    {
        String text;

        if (i == m_selectedIndex)
        {
            text += "> ";
        }
        else
        {
            text += "  ";
        }

        // Show Directories and books differently
        if (m_entries[i].isDirectory)
        {
            text += "[";
            text += m_entries[i].name;
            text += "]";
        }
        else
        {
            // If Book is not supported
            if (!IsSupportedBook(m_entries[i].name))
            {
                text += "X ";
            }
            text += m_entries[i].name;
        }

        m_display.DrawText(
            0,
            y,
            text,
            m_deviceSettings.UIFont);

        y +=
            m_layout.ItemHeight;
    }
}

void LibraryScreen::DrawGridView()
{
    // placeholder
}

ScreenCommand LibraryScreen::HandleButton(Button button)
{
    switch (button)
    {
    case Button::Up:
        MoveUp();
        Refresh(RefreshMode::Partial);
        break;
    case Button::Down:
        MoveDown();
        Refresh(RefreshMode::Partial);
        break;
    case Button::Left:
        MoveLeft();
        Refresh(RefreshMode::Partial);
        break;
    case Button::Right:
        MoveRight();
        Refresh(RefreshMode::Partial);
        break;
    case Button::Select:
        return OpenSelectedEntry();
        break;
    default:
        break;
    }

    return {};
}

void LibraryScreen::MoveUp()
{
    size_t count = m_entries.size();

    if (count <= 0)
    {
        return;
    }

    if (m_selectedIndex > 0)
    {
        m_selectedIndex--;
    }
    else
    {
        m_selectedIndex = count - 1;
    }

    EnsureSelectionVisible();
}

void LibraryScreen::MoveDown()
{
    auto count = m_entries.size();

    if (count <= 0)
    {
        return;
    }

    if (m_selectedIndex < count - 1)
    {
        m_selectedIndex++;
    }
    else
    {
        m_selectedIndex = 0;
    }

    EnsureSelectionVisible();
}

void LibraryScreen::MoveLeft()
{
    if (m_libSettings.LibraryView == LibraryViewMode::List)
    {
        return;
    }
    else
    {
        auto count = m_entries.size();
        if (m_selectedIndex > 0)
        {
            m_selectedIndex--;
        }
        else
        {
            m_selectedIndex = count - 1;
        }
    }
}

void LibraryScreen::MoveRight()
{
    if (m_libSettings.LibraryView == LibraryViewMode::List)
    {
        return;
    }
    else
    {
        auto count = m_entries.size();
        if (m_selectedIndex < count - 1)
        {
            m_selectedIndex++;
        }
        else
        {
            m_selectedIndex = 0;
        }
    }
}

ScreenCommand LibraryScreen::OpenSelectedEntry()
{
    if (m_entries.empty())
    {
        return {};
    }

    const auto &entry = m_entries[m_selectedIndex];

    if (entry.isDirectory)
    {
        LoadDirectory(entry.path);
        return {};
    }

    BookInfo book;
    book.path = entry.path;
    book.title = entry.name;
    return {
        ScreenCommandType::OpenBook,
        book};
}
