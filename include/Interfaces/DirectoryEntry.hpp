#pragma once

struct DirectoryEntry
{
    std::string name;

    std::string path;

    bool isDirectory;

    uint32_t size;

    uint32_t modifiedTime;
};