#pragma once
#include <ESPAsyncWebServer.h>
#include <vector>
#include <SPIFFS.h>

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
    std::vector<float> temperatures = getTemperatures();
    std::vector<std::string> jsonTemperatures;

    for (int i = 0; i < temperatures.size(); i++)
    {
        char buffer[10];
        sprintf(buffer, "%.4f", temperatures[i]);
        jsonTemperatures.push_back(buffer);
    }

    std::string json = "[";
    for (int i = 0; i < jsonTemperatures.size(); i++)
    {
        json += jsonTemperatures[i];
        if (i < jsonTemperatures.size() - 1)
        {
            json += ",";
        }
    }
    json += "]";

    request->send(200, "application/json", json.c_str());
}

void setupWebServer()
{
    if (!SPIFFS.begin())
    {
        Serial.println("Failed to mount file system");
        return;
    }
    loadHtmlFromFile();

    server.on("/", HTTP_GET, handleRootRequest);
    server.on("/temperatures", HTTP_GET, handleTemperaturesRequest);
    server.begin();
}