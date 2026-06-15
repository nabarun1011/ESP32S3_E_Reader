// #pragma once

// #include <vector>
// #include <BookInfo.h>
// #include <InputActions.h>

// struct PageStart
// {
//     int paragraphIndex;
//     int characterIndex;
// };

// struct BookState
// {
//     BookInfo bookInfo;

//     int currentPage;
//     int totalPages;
//     int lineHeight;
//     std::vector<String> paragraphs;
//     std::vector<PageStart> PageStarts;
//     std::vector<String> wrappedCurrentPageLines;
// };

// enum ReaderInputResults
// {
//     InputResult_None,
//     InputResult_Back,
// };

// class Reader
// {
// private:
//     BookState currentBook;
//     int LineSpacing;

// public:
//     void OpenBook(const BookInfo &bookInfo);
//     void ParseBook(String text);
//     void BuildPages();
//     void BuildCurrentPageLines();
//     void Render();
//     ReaderInputResults HandleInput(InputAction action);
// };