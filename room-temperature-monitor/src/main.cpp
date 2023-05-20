#include <Arduino.h>
#include "wifi.hpp"
#include "web_server.hpp"

void setup(void)
{
  Serial.begin(115200);
  delay(1000);

  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }
  connectToWifi();
  setupWebServer();

  Serial.println();
  Serial.println("Init OK");
}

void loop(void)
{
  delay(1000);
}
