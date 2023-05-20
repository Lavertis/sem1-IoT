#include <Arduino.h>
#include "lib/wifi.h"
#include "lib/web_server.h"

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
