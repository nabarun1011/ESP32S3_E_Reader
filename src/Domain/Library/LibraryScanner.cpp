#include "Domain/Library/LibraryScanner.hpp"

static const char* TAG = "LibraryScanner";

LibraryScanner::LibraryScanner(
    IStorage &storage)
    : m_storage(storage)
{
}

//Check if file is supported
static bool IsSupportedBook(
    const String &filename)
{
    String lower = filename;

    lower.toLowerCase();

    return lower.endsWith(".txt") ||
           lower.endsWith(".md") ||
           lower.endsWith(".epub");
}

bool LibraryScanner::Scan()
{
    m_books.clear();

    ScanDirectory("/");

    return true;
}

//Scan directory for books
void LibraryScanner::ScanDirectory(
    const String &path)
{
    auto entries = m_storage.ListDirectory(path);

    for (const auto &entry : entries)
    {
        
        if (entry.isDirectory)
        {
            if (entry.path == "/System Volume Information")
            {
                continue;
            }

            // Recursive scanning for sub directories
            ScanDirectory(entry.path);

            continue;
        }

        // If File format not supported then skip
        if (!IsSupportedBook(entry.name))
        {
            continue;
        }

        BookInfo book;

        book.title = entry.name;

        book.path = entry.path;

        int dot = book.title.lastIndexOf('.');

        if (dot >= 0)
        {
            book.extension = book.title.substring(dot + 1);
            book.title = book.title.substring(0, dot);
        }

        m_books.push_back(book);
    }
}

const std::vector<BookInfo> &
LibraryScanner::GetBooks() const
{
    return m_books;
}