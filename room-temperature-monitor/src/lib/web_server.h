#pragma once
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <vector>
#include <ArduinoJson.h>
#include "temp_sensor.h"

String html;

void loadHtmlFromFile()
{
    File file = SPIFFS.open("/index.html", "r");
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return;
    }

    while (file.available())
    {
        html += file.readString();
    }
    file.close();

    Serial.println("Html loaded from file");
}

AsyncWebServer server(80);

void handleRootRequest(AsyncWebServerRequest *request)
{
    request->send(200, "text/html", html);
}

void handleTemperaturesRequest(AsyncWebServerRequest *request)
{
    std::vector<TemperatureInfo> temperatures = getTemperatures();
    DynamicJsonDocument doc(1024);

    JsonArray tempArray = doc.to<JsonArray>();
    for (TemperatureInfo &tempInfo : temperatures) {
        JsonObject tempObject = tempArray.createNestedObject();
        tempObject["temperature"] = tempInfo.temperature;
        tempObject["resolution"] = tempInfo.resolution;
        tempObject["address"] = tempInfo.address.c_str();
    }

    String json;
    serializeJson(tempArray, json);
    request->send(200, "application/json", json.c_str());
}

void setupWebServer()
{
    loadHtmlFromFile();

    server.on("/", HTTP_GET, handleRootRequest);
    server.on("/temperatures", HTTP_GET, handleTemperaturesRequest);
    server.begin();
}