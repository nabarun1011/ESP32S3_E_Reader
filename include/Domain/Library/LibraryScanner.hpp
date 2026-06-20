#pragma once

#include <vector>

#include "Domain/Library/BookInfo.hpp"
#include "Interfaces/IStorage.hpp"

class LibraryScanner
{
public:
    explicit LibraryScanner(IStorage &storage);

    bool Scan();

    const std::vector<BookInfo> &GetEntries() const;

private:
    IStorage &m_storage;

    std::vector<BookInfo> m_entries;

    void ScanDirectory(const String &path);
};