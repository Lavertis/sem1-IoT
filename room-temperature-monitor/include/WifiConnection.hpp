#pragma once
#include <WiFi.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

class WifiConnection
{
public:
    static void connect();

private:
    static void loadCredentials(String &ssid, String &password);
};
