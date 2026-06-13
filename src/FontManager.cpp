#include <FontManager.h>
#include <DeviceSettings.h>

// Define available fonts sizes for each font family
const GFXfont *monoSizes[] = {
    &FreeMono9pt7b,
    &FreeMono12pt7b};

const GFXfont *monoBoldSizes[] = {
    &FreeMonoBold9pt7b,
    &FreeMonoBold12pt7b};

const GFXfont *sansSizes[] = {
    &FreeSans9pt7b,
    &FreeSans12pt7b};

const GFXfont *sansBoldSizes[] = {
    &FreeSansBold9pt7b,
    &FreeSansBold12pt7b};

const GFXfont *serifSizes[] = {
    &FreeSerif9pt7b,
    &FreeSerif12pt7b};

const GFXfont *serifBoldSizes[] = {
    &FreeSerifBold9pt7b,
    &FreeSerifBold12pt7b};

const GFXfont *GetCurrentFont(int size)
{
    // Get the current font size
    int sizeIndex = size == -1 ? DeviceSettingsMenuInstance.settings.UIFontSize : size;
        // Return the current font based on reader settings
    switch (DeviceSettingsMenuInstance.settings.UIFontFamily)
    {
    case Font_Mono:
        return monoSizes[sizeIndex];
        break;
    case Font_Sans:
        return sansSizes[sizeIndex];
        break;
    case Font_Serif:
        return serifSizes[sizeIndex];
        break;
    default:
        return nullptr;
        break;
    }
}

