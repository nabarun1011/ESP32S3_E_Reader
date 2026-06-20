#pragma once

#include <Arduino.h>
#include "Interfaces/IFile.hpp"
#include "vector"
#include "DirectoryEntry.hpp"
#include "memory"
#include "Interfaces/FileMode.hpp"

class IStorage
{
public:
    virtual ~IStorage() = default;

    virtual bool Init() = 0;

    virtual bool Exists(const String &path) = 0;

    virtual bool IsDirectory(const String &path) = 0;

    virtual size_t FileSize(const String &path) = 0;

    virtual std::unique_ptr<IFile> Open(const String &path, FileMode mode) = 0;

    virtual std::vector<DirectoryEntry> ListDirectory(const String &path) = 0;
};