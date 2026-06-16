#include "Domain/Reader/TextDocument.hpp"

TextDocument::TextDocument()
{
}

bool TextDocument::Open(std::unique_ptr<IFile> file)
{
    m_file = std::move(file);

    return m_file &&
           m_file->IsOpen();
}

bool TextDocument::IsOpen()
{
    return m_file &&
           m_file->IsOpen();
}

uint32_t TextDocument::Size()
{
    return m_file
               ? m_file->Size()
               : 0;
}

uint32_t TextDocument::Position()
{
    return m_file
               ? m_file->Position()
               : 0;
}

bool TextDocument::Seek(
    uint32_t position)
{
    return m_file &&
           m_file->Seek(position);
}

size_t TextDocument::Read(
    char *buffer,
    size_t size)
{
    if (!m_file)
    {
        return 0;
    }

    return m_file->Read(
        buffer,
        size);
}