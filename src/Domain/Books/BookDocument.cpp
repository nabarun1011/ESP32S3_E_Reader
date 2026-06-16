#include "Domain/Books/BookDocument.hpp"

BookDocument::BookDocument(
    std::unique_ptr<IFile> file)
    : m_file(std::move(file))
{
}

bool BookDocument::IsOpen() const
{
    return m_file &&
           m_file->IsOpen();
}

uint32_t BookDocument::Size() const
{
    return m_file
               ? m_file->Size()
               : 0;
}