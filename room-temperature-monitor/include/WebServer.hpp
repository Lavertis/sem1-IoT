#pragma once
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <vector>
#include <ArduinoJson.h>
#include "TemperatureSensor.hpp"

class WebServer
{
public:
    static void setup();

private:
    static void loadHtmlFromFile(String &html);
    static void handleRootRequest(AsyncWebServerRequest *request, String html);
    static void handleTemperaturesRequest(AsyncWebServerRequest *request);

    static AsyncWebServer server;
};
