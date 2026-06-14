#pragma once

#include "Interfaces/IStorage.hpp"
#include "SdFat.h"

class SDStorage : public IStorage
{
private:
    SdFat32 m_sd;
    SPIClass m_spi{FSPI};

public:
    bool Init() override;

    bool Exists(const String &path) override;

    bool IsDirectory(const String &path) override;

    size_t FileSize(const String &path) override;

    IFile *Open(const String &path) override;
};