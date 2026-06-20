#include "Platform/Storage/BookSettingsRepository.hpp"

BookSettingsRepository::BookSettingsRepository(
    IStorage &storage)
    : m_storage(storage)
{
}

String BookSettingsRepository::SettingsPath(
    const String &bookPath) const
{
    return bookPath + ".settings";
}

bool BookSettingsRepository::Save(
    const String &bookPath,
    const BookSettings &settings)
{
    auto file =
        m_storage.Open(
            SettingsPath(bookPath),
            FileMode::Write);

    if (!file)
    {
        return false;
    }

    String content;

    content +=
        "font=" +
        String(
            static_cast<int>(
                settings.Font)) +
        "\n";

    content +=
        "fontsize=" +
        String(
            settings.FontSize) +
        "\n";

    content +=
        "offset=" +
        String(
            settings.LastCharacterOffset) +
        "\n";

    file->Write(
        content.c_str(),
        content.length());

    file->Close();

    return true;
}

bool BookSettingsRepository::Load(
    const String &bookPath,
    BookSettings &settings)
{
    auto file =
        m_storage.Open(
            SettingsPath(bookPath),
            FileMode::Read);

    if (!file)
    {
        return false;
    }

    char buffer[256];

    size_t bytesRead =
        file->Read(
            buffer,
            sizeof(buffer) - 1);

    buffer[bytesRead] = '\0';

    file->Close();

    String content =
        buffer;

    // Parse Font
    int fontStart =
        content.indexOf("font=");

    if (fontStart >= 0)
    {
        int end =
            content.indexOf(
                '\n',
                fontStart);

        settings.Font =
            static_cast<ReaderFont>(
                content.substring(
                           fontStart + 5,
                           end)
                    .toInt());
    }

    // Parse Font Size
    int fontSizeStart =
        content.indexOf(
            "fontsize=");

    if (fontSizeStart >= 0)
    {
        int end =
            content.indexOf(
                '\n',
                fontSizeStart);

        settings.FontSize =
            content.substring(
                       fontSizeStart + 9,
                       end)
                .toInt();
    }
    // Parse Page offset
    int offsetStart =
        content.indexOf(
            "offset=");

    if (offsetStart >= 0)
    {
        int end =
            content.indexOf(
                '\n',
                offsetStart);

        settings.LastCharacterOffset =
            content.substring(
                       offsetStart + 7,
                       end)
                .toInt();
    }

    return true;
}