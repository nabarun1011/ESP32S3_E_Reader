#include "Platform/Storage/SDFile.hpp"

SDFile::SDFile(
    FsFile &&file)
    : m_file(std::move(file))
{
}

bool SDFile::IsOpen() const
{
    return m_file.isOpen();
}

size_t SDFile::Read(
    void *buffer,
    size_t size)
{
    return m_file.read(
        buffer,
        size);
}

bool SDFile::Seek(
    uint32_t position)
{
    return m_file.seek(position);
}

uint32_t SDFile::Position() const
{
    return m_file.curPosition();
}

uint32_t SDFile::Size()
{
    return m_file.size();
}

void SDFile::Close()
{
    m_file.close();
}

size_t SDFile::Write(
    const void *buffer,
    size_t size)
{
    return m_file.write(
        static_cast<const uint8_t *>(buffer),
        size);
}