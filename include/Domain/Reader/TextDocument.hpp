// Owns a file
// Provides book text
// Tracks reading position

#pragma once

#include <memory>

#include "Interfaces/IFile.hpp"
#include "Interfaces/IDocument.hpp"

class TextDocument : public IDocument
{
public:
    TextDocument();

    bool Open(std::unique_ptr<IFile> file);

    bool IsOpen() override;

    uint32_t Size();

    uint32_t Position();

    bool Seek(
        uint32_t position);

    size_t Read(
        char *buffer,
        size_t size);

    bool ReadPage(
        char *buffer,
        size_t maxSize);

    String ReadAll() override;

private:
    std::unique_ptr<IFile> m_file;
};