#pragma once
#include <ESPAsyncWebServer.h>
#include <vector>

String html = "<!DOCTYPE html>"
              "<html>"
              "<head>"
              "<title>Temperature Monitor</title>"
              "<style>"
              ".temperature-square {"
              "  display: inline-block;"
              "  width: 100px;"
              "  height: 100px;"
              "  margin: 10px;"
              "  text-align: center;"
              "  font-weight: bold;"
              "}"
              "body {"
              "  display: flex;"
              "  justify-content: center;"
              "  align-items: center;"
              "  height: 60vh;"
              "  margin: 0;"
              "}"
              "</style>"
              "</head>"
              "<body>"
              "<div>"
              "<h1 style='text-align: center;'>Temperature Monitor</h1>"
              "<div id='temperature-container'></div>"
              "<script>"
              "function createTemperatureSquare(temperature) {"
              "  var square = document.createElement('div');"
              "  square.className = 'temperature-square';"
              "  square.style.backgroundColor = val2hsl(temperature, 25, 30);"
              "  square.textContent = temperature;"
              "  return square;"
              "}"
              "function val2hsl(value, min, max) {"
              " if (value < min) {"
              " value = min;"
              " } else if (value > max) {"
              " value = max;"
              " }"
              ""
              " let hue = (1 - ((value - min) / (max - min))) * 240;"
              " return `hsl(${hue}, 100%, 50%)`;"
              "}"
              "function updateTemperatureDisplay(data) {"
              "var container = document.getElementById('temperature-container');"
              "container.innerHTML = '';"
              "var row = document.createElement('div');"
              "row.className = 'temperature-row';"
              "for (var i = 0; i < data.length; i++) {"
              "var square = createTemperatureSquare(data[i]);"
              "row.appendChild(square);"
              "if ((i + 1) % 4 === 0) {"
              "container.appendChild(row);"
              "row = document.createElement('div');"
              "row.className = 'temperature-row';"
              "}"
              "}"
              "if (row.children.length > 0) {"
              "container.appendChild(row);"
              "}"
              "}"
              "setInterval(function() {"
              "  fetch('/temperatures')"
              "    .then(response => response.json())"
              "    .then(data => {"
              "      updateTemperatureDisplay(data);"
              "    });"
              "}, 3000);" // Update temperature every 1 second
              "</script>"
              "</div>"
              "</body>"
              "</html>";

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
    server.on("/", HTTP_GET, handleRootRequest);
    server.on("/temperatures", HTTP_GET, handleTemperaturesRequest);
    server.begin();
}