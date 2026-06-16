#pragma once

#include <memory>

#include "Interfaces/IFile.hpp"

class BookDocument
{
public:
    explicit BookDocument(
        std::unique_ptr<IFile> file);

    bool IsOpen() const;

    uint32_t Size() const;

private:
    std::unique_ptr<IFile> m_file;
};