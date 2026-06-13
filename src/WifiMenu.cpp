#include <WiFiMenu.h>
#include <RenderSystem.h>
#include <Pins.h>
#include <UIRenderer.h>

WifiMenu::WifiMenu()
{
    selectedSSIDIndex = 0;
}

void WifiMenu::Begin()
{
    ScanNetworks();
}

void WifiMenu::ScanNetworks()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    int SSIDCount = WiFi.scanNetworks();

    Ssids.clear();

    for (int i = 0; i < SSIDCount; ++i)
    {
        Ssids.push_back(WiFi.SSID(i));
    }
}

void WifiMenu::Render()
{
    switch (currentMode)
    {
    case WifiMenuMode_SSIDs:
        RenderSSIDs();
        break;
    case WifiMenuMode_PasswordTyping:
        RenderPasswordTyping();
        break;
    default:
        break;
    }
}

void WifiMenu::RenderSSIDs()
{
    int y = headerHeight + 20;

    for (int i = 0; i < Ssids.size(); i++)
    {
        display.setCursor(10, y + i * 30);
        if (i == selectedSSIDIndex)
        {
            display.print(">");
        }
        else
        {
            display.print(" ");
        }
        display.println(Ssids[i]);
    }
}

void WifiMenu::RenderPasswordTyping()
{
    // display.setCursor(
    //     20,
    //     30);

    // display.println(
    //     "Enter Password");
    // display.setCursor(
    //     20,
    //     60);
    // display.println(
    //     password);

    // int StartY = 100;

    // for (int Row = 0;
    //      Row < 6;
    //      Row++)
    // {
    //     int StartX = 20;

    //     for (int Column = 0;
    //          Column < 6;
    //          Column++)
    //     {
    //         char Key =
    //             KeyboardRows[Row][Column];
    //         display.setCursor(
    //             StartX,
    //             StartY);

    //         if (
    //             Row == keyBoardRow &&
    //             Column == keyBoardCol)
    //         {
    //             display.print(">");
    //         }
    //         else
    //         {
    //             display.print(" ");
    //         }
    //         display.print(Key);

    //         StartX += 35;
    //     }

    //     StartY += 30;
    // }
}

WifiMenuResults WifiMenu::HandleInput(InputAction action)
{
    switch (currentMode)
    {
    case WifiMenuMode_SSIDs:
        if (HandleSSIDSelection(action) == WifiMenuResult_Back)
        {
            return WifiMenuResult_Back;
        }
        break;
    case WifiMenuMode_PasswordTyping:
        HandlePasswordTyping(action);
        break;
    default:
        break;
    }
    return WifiMenuResult_None;
}

WifiMenuResults WifiMenu::HandleSSIDSelection(InputAction action)
{
    switch (action)
    {
    case Action_UP:
        selectedSSIDIndex--;
        if (selectedSSIDIndex < 0)
        {
            selectedSSIDIndex = Ssids.size() - 1;
        }
        RequestRefresh(Refresh_Full);
        break;
    case Action_DOWN:
        selectedSSIDIndex++;
        if (selectedSSIDIndex >= Ssids.size())
        {
            selectedSSIDIndex = 0;
        }
        RequestRefresh(Refresh_Full);
        break;
    case Action_SELECT:
        Connect(Ssids[selectedSSIDIndex], "");
        currentMode = WifiMenuMode_PasswordTyping;
        break;
    case Action_BACK:
        return WifiMenuResult_Back;
        break;
    default:
        break;
    }

    return WifiMenuResult_None;
}

void WifiMenu::HandlePasswordTyping(InputAction action)
{
    // switch (action)
    // {
    // case Action_UP:
    //     keyBoardRow--;
    //     if (keyBoardRow < 0)
    //     {
    //         keyBoardRow = Keyboard.size() - 1;
    //     }
    //     break;
    // case Action_DOWN:
    //     keyBoardRow++;
    //     if (keyBoardRow > Keyboard.size() - 1)
    //     {
    //         keyBoardRow = 0;
    //     }
    //     break;
    // case Action_LEFT:
    //     keyBoardCol--;
    //     if (keyBoardCol < 0)
    //     {
    //         keyBoardCol = Keyboard[keyBoardRow].size() - 1;
    //     }
    //     break;
    // case Action_RIGHT:
    //     keyBoardCol++;
    //     if (keyBoardCol > Keyboard[keyBoardRow].size() - 1)
    //     {
    //         keyBoardCol = 0;
    //     }
    //     break;
    // case Action_SELECT:
    //     password += Keyboard[keyBoardRow][keyBoardCol];

    //     break;
    // case Action_BACK:
    //     if (password.length() > 0)
    //     {
    //         password.remove(password.length() - 1);
    //     }
    //     break;
    // default:
    //     break;
    // }
}

String WifiMenu::GetSelectedSsid()
{
    return Ssids[selectedSSIDIndex];
}

bool WifiMenu::Connect(String ssid, String password)
{
    WiFi.begin(ssid.c_str(), password.c_str());

    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 15000)
    {
        delay(100);
    }

    return WiFi.status() == WL_CONNECTED;
}
