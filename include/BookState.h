#pragma once
#include <Arduino.h>
#include <ReaderSettings.h>
struct BookState
{
    String bookPath;

    int currentPage;
    int totalPages;

    ReaderSettings readerSettings;
};