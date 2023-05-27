#include "WebServer.hpp"

AsyncWebServer WebServer::server(80);
String WebServer::rootHtml;

void WebServer::setup()
{
    loadRootHtmlFromFile();

    server.on("/", HTTP_GET, handleRootRequest);
    server.on("/temperatures", HTTP_GET, handleTemperaturesRequest);
    server.begin();
}

void WebServer::handleRootRequest(AsyncWebServerRequest *request)
{
    request->send(200, "text/html", rootHtml);
}

void WebServer::handleTemperaturesRequest(AsyncWebServerRequest *request)
{
    request->send(200, "application/json", TemperatureSensor::getTemperaturesAsJson());
}

void WebServer::loadRootHtmlFromFile()
{
    File file = SPIFFS.open("/index.html", "r");
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return;
    }

    while (file.available())
    {
        rootHtml += file.readString();
    }
    file.close();

    Serial.println("Html loaded from file");
}
