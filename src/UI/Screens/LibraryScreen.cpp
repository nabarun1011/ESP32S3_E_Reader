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
    LoadDirectory(m_rootBookPath);
}

void LibraryScreen::Exit()
{
    Serial.printf("%s : LibraryScreen Exit\n", TAG);
}
void LibraryScreen::RebuildLayout()
{
    switch (m_libSettings.LibraryView)
    {
    case LibraryViewMode::List:

        m_layout.Columns = 1;

        // Height of each book entry in list view
        m_layout.ItemHeight = m_display.LineHeight(m_deviceSettings.UIFont);

        // Number of visible book entries
        m_layout.VisibleRows = m_display.Height() / m_layout.ItemHeight;

        break;

    case LibraryViewMode::Grid:

        // placeholder

        m_layout.Columns = 2;

        m_layout.ItemHeight = 80;

        m_layout.VisibleRows = m_display.Height() / m_layout.ItemHeight;

        break;
    }
}

void LibraryScreen::EnsureSelectionVisible()
{
    // Number of visible items
    size_t visibleItems = m_layout.Columns * m_layout.VisibleRows;

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

void LibraryScreen::Refresh()
{
    RebuildLayout();
    switch (m_libSettings.LibraryView)
    {
    case LibraryViewMode::List:
        DrawListView();
        break;
    case LibraryViewMode::Grid:
        DrawGridView();
        break;
    default:
        break;
    }
}

void LibraryScreen::LoadDirectory(const String &path)
{
    m_currentPath = path;

    m_entries = m_storage.ListDirectory(path);

    SortEntries();

    m_selectedIndex = 0;

    m_firstVisibleIndex = 0;

    Refresh();
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

            switch (m_libSettings.LibrarySorting)
            {
            case LibrarySort::NameAscending:
                return a.name < b.name;

            case LibrarySort::NameDescending:
                return a.name > b.name;
            case LibrarySort::DateNewestFirst:
                return a.modifiedTime >b.modifiedTime;

            case LibrarySort::DateOldestFirst:
                return a.modifiedTime <b.modifiedTime;

            case LibrarySort::SizeLargestFirst:
                return a.size >b.size;

            case LibrarySort::SizeSmallestFirst:
                return a.size <b.size;
            default:
                return a.name < b.name;
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

void LibraryScreen::DrawListView()
{
    m_display.Render([this]()
                     {
        int contentWidth =    m_display.Width() -    m_layout.ScrollBarWidth -    4;
        const auto &books = m_entries;

        size_t visibleItems = m_layout.VisibleRows;

        size_t endIndex = std::min(
            books.size(),
            m_firstVisibleIndex +
                visibleItems);

        int y = m_layout.ItemHeight;

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

            //Show Directories and books differently
            if(m_entries[i].isDirectory)
            {
                text += "[";
                text += m_entries[i].name;
                text += "]";
            }
            else
            {

                if(!IsSupportedBook(m_entries[i].name))
                {
                    text += "X ";
                }
                text+=m_entries[i].name;
            }           

            m_display.DrawText(
                0,
                y,
                text,
                m_deviceSettings.UIFont);

            y +=
                m_layout.ItemHeight;
        } });
}

void LibraryScreen::DrawGridView()
{
    // placeholder
}

void LibraryScreen::HandleButton(Button button)
{
    bool needRedraw = true;
    switch (button)
    {
    case Button::Up:
        MoveUp();
        break;
    case Button::Down:
        MoveDown();
        break;
    case Button::Left:
        MoveLeft();
        break;
    case Button::Right:
        MoveRight();
        break;
    case Button::Select:
        OpenSelectedBook();
        break;
    default:
        break;
    }

    if (needRedraw)
    {
        Refresh();
    }
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

void LibraryScreen::OpenSelectedBook()
{
    if (m_entries.empty())
    {
        return;
    }

    const auto &entry = m_entries[m_selectedIndex];

    if (entry.isDirectory)
    {
        LoadDirectory(entry.path);
        return;
    }
    m_requestedBook.path = entry.path;
    m_requestedBook.title = entry.name;
    m_hasBookOpenRequest = true;
}

BookInfo LibraryScreen::TakeBookOpenRequest()
{
    m_hasBookOpenRequest = false;
    return m_requestedBook;
}

bool LibraryScreen::HasBookOpenRequest() const
{
    return m_hasBookOpenRequest;
}