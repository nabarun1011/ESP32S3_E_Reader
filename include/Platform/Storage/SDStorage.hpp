#pragma once

#include "Interfaces/IStorage.hpp"
#include "SdFat.h"

class SDStorage : public IStorage
{
private:
public:
    SdFs m_sd;

    explicit SDStorage();

    bool Init() override;

    bool Exists(const String &path) override;

    bool IsDirectory(const String &path) override;

    size_t FileSize(const String &path) override;

    std::unique_ptr<IFile> Open(const String &path, FileMode mode) override;

    std::vector<DirectoryEntry> ListDirectory(const String &path) override;
};