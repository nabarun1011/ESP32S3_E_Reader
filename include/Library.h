#pragma once

#include <vector>
#include <BookInfo.h>
#include <InputActions.h>

enum LibraryResults
{
    LibraryResult_None,
    LibraryResult_OpenBook,
    LibraryResult_OpenSettings
};

class Library
{
private:
    std::vector<BookInfo> libraryBooks;
    int selectedBookIndex = 0;

public:
    Library();
    void ScanAllBooks();
    void ScanFlashBooks();
    void ScanSDBooks();
    void Render();
    LibraryResults HandleInput(InputAction action);
    BookInfo GetSelectedBook();
};

extern Library library;