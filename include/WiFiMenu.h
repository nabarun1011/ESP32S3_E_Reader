#pragma once

#include <WiFi.h>
#include <vector>
#include <InputActions.h>

enum WifiMenuMode
{
    WifiMenuMode_SSIDs,
    WifiMenuMode_PasswordTyping
};

enum WifiMenuResults
{
    WifiMenuResult_None,
    WifiMenuResult_Back
};


class WifiMenu
{
private:
    std::vector<String> Ssids;
    int selectedSSIDIndex = 0;
    String password = "";
    int keyBoardRow = 0;
    int keyBoardCol = 0;
    WifiMenuMode currentMode = WifiMenuMode_SSIDs;

public:
    WifiMenu();
    void Begin();
    void ScanNetworks();
    void Render();
    void RenderSSIDs();
    void RenderPasswordTyping();
    WifiMenuResults HandleInput(InputAction action);
    WifiMenuResults HandleSSIDSelection(InputAction action);
    void HandlePasswordTyping(InputAction action);
    String GetSelectedSsid();
    bool Connect(String ssid, String password);
};