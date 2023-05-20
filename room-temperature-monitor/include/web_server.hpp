#pragma once
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <vector>
#include <ArduinoJson.h>
#include "temp_sensor.hpp"

void loadHtmlFromFile(String &html);
void handleRootRequest(AsyncWebServerRequest *request, String html);
void handleTemperaturesRequest(AsyncWebServerRequest *request);
void setupWebServer();
