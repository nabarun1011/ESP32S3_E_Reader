#include "Platform/Storage/SDFile.hpp"

SDFile::SDFile(
    FsFile &&file)
    : m_file(std::move(file))
{
}

bool SDFile::IsOpen() 
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

uint32_t SDFile::Position() 
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