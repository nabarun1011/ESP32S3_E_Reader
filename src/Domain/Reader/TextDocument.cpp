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

size_t TextDocument::Size()
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

String TextDocument::ReadAll()
{
    if (!m_file)
    {
        return "";
    }

    const size_t size = m_file->Size();

    if (size == 0)
    {
        return "";
    }

    String text;

    text.reserve(size);

    m_file->Seek(0);

    constexpr size_t BufferSize = 512;

    char buffer[BufferSize];
    Serial.println("ReadAll start");

    while (true)
    {
        size_t bytesRead =
            m_file->Read(
                buffer,
                BufferSize);

        if (bytesRead == 0)
        {
            break;
        }

        Serial.printf(
            "Read %u bytes, pos=%u\n",
            (unsigned)bytesRead,
            (unsigned)m_file->Position());

        for (size_t i = 0;
             i < bytesRead;
             ++i)
        {
            text += buffer[i];
        }
    }

    Serial.println("ReadAll done");

    return text;
}