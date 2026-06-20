#pragma once

#include <SdFat.h>

#include "Interfaces/IFile.hpp"

class SDFile : public IFile
{
public:
    explicit SDFile(
        FsFile &&file);

    bool IsOpen() const override;

    size_t Read(void *buffer, size_t size) override;

    size_t Write(const void *buffer, size_t size) override;

    bool Seek(
        uint32_t position) override;

    uint32_t Position() const override;

    uint32_t Size() override;

    void Close() override;

private:
    FsFile m_file;
};