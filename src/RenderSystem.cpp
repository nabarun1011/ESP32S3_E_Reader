// #include <RenderSystem.h>
// #include <App.h>
// #include <Pins.h>
// #include <FontManager.h>
// #include <DeviceSettings.h>
// #include <UIRenderer.h>

// RefreshType pendingRefresh = Refresh_None;

// void RequestRefresh(RefreshType type)
// {
//     if (type != Refresh_None)
//     {
//         pendingRefresh = type;
//     }
// }

// void ProcessRender()
// {
//     if (pendingRefresh == Refresh_None)
//     {
//         return;
//     }

//     display.setRotation(DeviceSettingsMenuInstance.settings.orientation);

//     if (pendingRefresh == Refresh_Full)
//     {
//         display.setFullWindow();
//     }
//     else if (pendingRefresh == Refresh_UpperHalf)
//     {
//         display.setPartialWindow(0, display.height() / 2, display.width(), display.height() / 2);
//     }
//     else if (pendingRefresh == Refresh_LowerHalf)
//     {
//         display.setPartialWindow(0, 0, display.width(), display.height() / 2);
//     }
//     else if (pendingRefresh == Refresh_Header)
//     {
//         display.setPartialWindow(0, 0, display.width(), headerHeight);
//     }
//     display.firstPage();
//     do
//     {
//         appInstance.RenderCurrentScreen();
//     } while (display.nextPage());

//     pendingRefresh = Refresh_None;
// }