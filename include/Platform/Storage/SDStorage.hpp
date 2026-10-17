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

    bool Exists(const std::string &path) override;

    bool IsDirectory(const std::string &path) override;

    size_t FileSize(const std::string &path) override;

    std::unique_ptr<IFile> Open(const std::string &path, FileMode mode) override;

    std::vector<DirectoryEntry> ListDirectory(const std::string &path) override;
};