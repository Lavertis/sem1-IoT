#include <Arduino.h>
#include "WifiConnection.hpp"
#include "WebServer.hpp"

void setup(void)
{
  Serial.begin(115200);
  delay(1000);

  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }

  WifiConnection::connect();
  WebServer::setup();
  TemperatureSensor::setResolution(11);

  Serial.println();
  Serial.println("Init OK");
}

void loop(void)
{
  TemperatureSensor::update();
}