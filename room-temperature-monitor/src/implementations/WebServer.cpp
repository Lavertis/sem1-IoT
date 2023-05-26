#include "WebServer.hpp"

AsyncWebServer WebServer::server(80);

void WebServer::setup()
{
    String rootHtml;
    loadHtmlFromFile(rootHtml);

    server.on("/", HTTP_GET, [rootHtml](AsyncWebServerRequest *request)
              { handleRootRequest(request, rootHtml); });
    server.on("/temperatures", HTTP_GET, handleTemperaturesRequest);
    server.begin();
}

void WebServer::handleRootRequest(AsyncWebServerRequest *request, String html)
{
    request->send(200, "text/html", html);
}

void WebServer::handleTemperaturesRequest(AsyncWebServerRequest *request)
{
    std::vector<TemperatureInfo> temperatures = TemperatureSensor::getTemperatures();
    DynamicJsonDocument doc(1024);

    JsonArray tempArray = doc.to<JsonArray>();
    for (TemperatureInfo &tempInfo : temperatures)
    {
        JsonObject tempObject = tempArray.createNestedObject();
        tempObject["temperature"] = tempInfo.temperature;
        tempObject["resolution"] = tempInfo.resolution;
        tempObject["address"] = tempInfo.address.c_str();
    }

    String json;
    serializeJson(tempArray, json);
    request->send(200, "application/json", json);
}

void WebServer::loadHtmlFromFile(String &html)
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