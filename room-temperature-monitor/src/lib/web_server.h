#pragma once
#include <ESPAsyncWebServer.h>

String html = "<!DOCTYPE html>"
              "<html>"
              "<head>"
              "<title>Hello World</title>"
              "</head>"
              "<body>"
              "<h1>Hello World</h1>"
              "<p>Text</p>"
              "</body>"
              "</html>";

void handleRootRequest(AsyncWebServerRequest *request)
{
    request->send(200, "text/html", html);
}

AsyncWebServer server(80);

void setupWebServer()
{
    server.on("/", HTTP_GET, handleRootRequest);
    server.begin();
}