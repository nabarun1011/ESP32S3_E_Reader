// #include <Library.h>
// #include <SD.h>
// #include <Pins.h>
// // #include <LittleFS.h>
// #include <RenderSystem.h>
// #include <UIRenderer.h>
// #include <FontManager.h>

// // SPIClass spiSD(FSPI);

// Library library;

// Library::Library()
// {
//     selectedBookIndex = 0;
// }

// void Library::ScanAllBooks()
// {
//     libraryBooks.clear();
//     // libraryBooks = {};
//     // ScanFlashBooks();
//     Serial.println("Scanning books...");
//     spiSD.begin(SPI_CLK, SPI_MISO, SPI_MOSI, SD_CS);
//     if (SD.begin(SD_CS, spiSD))
//     {
//         Serial.println("SD Card found.");
//         ScanSDBooks();
//     }
// }

// // void Library::ScanFlashBooks()
// // {
// //     File root = LittleFS.open("/");

// //     File file = root.openNextFile();

// //     while (file)
// //     {
// //         String fileName = file.name();

// //         if (fileName.endsWith(".txt"))
// //         {
// //             BookInfo bookInfo;
// //             bookInfo.path = "/" + fileName;
// //             bookInfo.title = fileName.substring(fileName.lastIndexOf("/") + 1, fileName.lastIndexOf(".txt"));
// //             bookInfo.storage = &LittleFS;
// //             libraryBooks.push_back(bookInfo);
// //         }

// //         file = root.openNextFile();
// //     }
// // }

// void Library::ScanSDBooks()
// {
//     File root = SD.open("/");

//     File file = root.openNextFile();

//     while (file)
//     {
//         String fileName = file.name();

//         if (fileName.endsWith(".txt"))
//         {
//             BookInfo bookInfo;
//             bookInfo.path = "/" + fileName;
//             bookInfo.title = fileName.substring(fileName.lastIndexOf("/") + 1, fileName.lastIndexOf(".txt"));
//             bookInfo.storage = &SD;
//             libraryBooks.push_back(bookInfo);
//         }

//         file = root.openNextFile();
//     }
// }

// void Library::Render()
// {

//     display.setTextWrap(true);
//     display.setFont(GetCurrentFont());

//     int y = headerHeight + 20;

//     for (int i = 0; i < libraryBooks.size(); i++)
//     {
//         display.setCursor(20, y);
//         if (i == selectedBookIndex)
//         {
//             display.print(">");
//         }
//         else
//         {
//             display.print(" ");
//         }
//         display.println(libraryBooks[i].title);
//         y += 40;
//     }
// }

// LibraryResults Library::HandleInput(InputAction action)
// {
//     switch (action)
//     {
//     case Action_UP:
//         if (selectedBookIndex > 0)
//         {
//             selectedBookIndex--;
//             if (selectedBookIndex < 0)
//             {
//                 selectedBookIndex = 0;
//             }
//         }
//         break;
//     case Action_DOWN:
//         if (selectedBookIndex < libraryBooks.size() - 1)
//         {
//             selectedBookIndex++;
//             if (selectedBookIndex >= libraryBooks.size())
//             {
//                 selectedBookIndex =
//                     libraryBooks.size() - 1;
//             }
//         }
//         break;
//     case Action_LEFT:
//         // Handle left action (e.g., navigate back or open settings)
//         break;
//     case Action_RIGHT:
//         // Handle right action (e.g., navigate forward or open book details)
//         break;
//     case Action_SELECT:
//         return LibraryResult_OpenBook;
//         break;
//     case Action_BACK:
//         return LibraryResult_OpenSettings;
//         break;
//     }
//     return LibraryResult_None;
// }

// BookInfo Library::GetSelectedBook()
// {
//     return libraryBooks[selectedBookIndex];
// }