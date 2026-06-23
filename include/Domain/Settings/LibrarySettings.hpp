#pragma

enum class LibraryViewMode
{
    List,
    Grid
};

enum class LibrarySort
{
    NameAscending,
    NameDescending,

    DateNewestFirst,
    DateOldestFirst,

    SizeLargestFirst,
    SizeSmallestFirst
};

struct LibrarySettings
{
    LibraryViewMode LibraryView = LibraryViewMode::List;

    LibrarySort LibrarySorting = LibrarySort::NameAscending;

    
};