#include <Arduino.h>
#include <DS18B20.h> // https://github.com/matmunk/DS18B20
DS18B20 ds(5);       // pin GPIO5
#include <WiFi.h>
#include "wifi_credentials.h"

uint8_t address[8];
char addressString[20];

void connectToWifi()
{
  WiFi.mode(WIFI_STA); // Optional
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("\nConnecting");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
}

void initDS18B20()
{
  ds.selectNext();
  ds.getAddress(address);
  for (int j = 0; j < sizeof(address); j++)
  {
    sprintf(&addressString[j * 2], "%02X", address[j]);
  }
}

void setup(void)
{
  Serial.begin(115200);
  delay(1000);

  initDS18B20();
  connectToWifi();

  Serial.println();
  Serial.println("Init OK");
}

void loop(void)
{
  Serial.print("Address: ");
  Serial.println(addressString);
  float temp = ds.getTempC();

  Serial.print("Temp: ");
  Serial.println(temp);

  delay(1000);
}