#pragma once
#include <WiFi.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

const int capacity = JSON_OBJECT_SIZE(2) + 100;

void loadWifiCredentials(String &ssid, String &password)
{
  File file = SPIFFS.open("/wifi_credentials.json", "r");
  if (!file)
  {
    Serial.println("Failed to open wifi credentials file");
    return;
  }

  // Read contents of the file
  String content;
  while (file.available())
  {
    content += file.readString();
  }
  file.close();
  Serial.println(content);

  // Parse JSON
  DynamicJsonDocument doc(capacity);
  DeserializationError error = deserializeJson(doc, content);
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    Serial.println("Failed to parse wifi credentials file");
    return;
  }

  // Get wifi credentials
  ssid = doc["ssid"].as<String>();
  password = doc["password"].as<String>();
}

void connectToWifi()
{
  String ssid, password;
  loadWifiCredentials(ssid, password); // Load the credentials

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.println("\nConnecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
}
