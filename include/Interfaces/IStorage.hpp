#pragma once

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

    virtual bool Exists(const std::string &path) = 0;

    virtual bool IsDirectory(const std::string &path) = 0;

    virtual size_t FileSize(const std::string &path) = 0;

    virtual std::unique_ptr<IFile> Open(const std::string &path, FileMode mode) = 0;

    virtual std::vector<DirectoryEntry> ListDirectory(const std::string &path) = 0;
};