#pragma once
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "TemperatureSensor.hpp"

class WebServer
{
public:
    static void setup();

private:
    static void loadRootHtmlFromFile();
    static void handleRootRequest(AsyncWebServerRequest *request);
    static void handleTemperaturesRequest(AsyncWebServerRequest *request);
    static void handleIpRequest(AsyncWebServerRequest *request);

    static AsyncWebServer server;
    static String rootHtml;
};
