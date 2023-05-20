#include <Arduino.h>
#include "lib/sensor_setup.h"
#include "lib/wifi_setup.h"
#include "lib/web_server.h"
#include "wifi_credentials.h"

void setup(void)
{
  Serial.begin(115200);
  delay(1000);

  initDS18B20();
  connectToWifi(WIFI_SSID, WIFI_PASSWORD);
  setupWebServer();

  Serial.println();
  Serial.println("Init OK");
}

void loop(void)
{
  delay(1000);
}
