#pragma once
#include <WiFi.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

void loadWifiCredentials(String &ssid, String &password);
void connectToWifi();
