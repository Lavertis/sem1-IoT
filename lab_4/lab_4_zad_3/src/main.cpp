#include <Arduino.h>

// 30.07.2022
// dla nodemcu-32s
// współpraca z TFT ili9341
#include <SPI.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
// Invoke library, pins defined in User_Setup.h

#include <DS18B20.h> //https://github.com/matmunk/DS18B20
DS18B20 ds(14);      // pin GPIO14

uint32_t count = 0;
uint8_t address[8];
char addressStr[20];
float temp = 1;

float temps[10];
int resolutions[10];

char addressStrings[10][20];

void setup(void)
{
  Serial.begin(115200);
  delay(1000);
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  Serial.println();
  Serial.print("TFT Init OK");
}

void loop(void)
{
  count++;

  tft.drawLine(tft.width() * 0.65, 0, tft.width() * 0.65, tft.height() * 0.1, TFT_GREEN);
  tft.fillRect(tft.width() * 0.655, 0, tft.width() * 0.35, tft.height() * 0.1, TFT_RED);
  tft.setCursor(tft.width() * 0.7, tft.height() * 0.03, 1);
  tft.setTextColor(TFT_YELLOW);
  tft.print(String(count));

  tft.drawLine(0, tft.height() * 0.1, tft.width(), tft.height() * 0.1, TFT_GREEN);
  tft.drawLine(0, tft.height() * 0.2, tft.width(), tft.height() * 0.2, TFT_GREEN);
  tft.drawLine(0, tft.height() * 0.89, tft.width(), tft.height() * 0.89, TFT_GREEN);

  tft.setCursor(tft.width() * 0.15, tft.height() * 0.13, 1);
  tft.setTextColor(TFT_GREEN);
  tft.print("Sensor DS18B20");

  int i = 0;

  while (ds.selectNext())
  {
    ds.getAddress(address);
    ds.setResolution(10 + i);
    temps[i] = ds.getTempC();

    // Clear
    tft.fillRect(0, tft.height() * (0.25 + i * 0.2), tft.width(), tft.height() * 0.2, TFT_BLACK);

    // Write
    tft.setTextColor(TFT_GREEN);
    tft.setCursor(tft.width() * 0.025, tft.height() * (0.25 + i * 0.2), 1);
    tft.print("T(" + String(i + 1) + ")=" + String(temps[i]) + " C");

    tft.setCursor(tft.width() * 0.75, tft.height() * (0.25 + i * 0.2), 1);
    tft.print("R:" + String(ds.getResolution()));

    for (int j = 0; j < sizeof(address); j++)
    {
      sprintf(&addressStrings[i][j * 2], "%02X", address[j]);
    }

    tft.setCursor(tft.width() * 0.025, tft.height() * (0.325 + i * 0.2), 1);
    tft.print(addressStrings[i]);
    tft.setCursor(tft.width() * 0.9, tft.height() * (0.325 + i * 0.2), 1);
    tft.print(ds.getPowerMode() ? "P" : "N");

    Serial.print("Sensor " + String(addressStrings[i]) + " ");
    Serial.print("Power mode: ");
    Serial.println(ds.getPowerMode() ? "Pasożytniczy" : "Normalny");

    i++;
  }

  // Czyszczenie pozostałych/wyjętych
  for (int j = i; j < 3; j++)
  {
    tft.setTextColor(TFT_BLACK);
    tft.fillRect(tft.width() * 0.025, tft.height() * (0.25 + j * 0.2), tft.width() * 0.95, tft.height() * 0.2, TFT_BLACK);
  }

  tft.setCursor(tft.width() * 0.125, tft.height() * 0.9, 2);
  tft.setTextColor(TFT_RED);
  tft.print("Laboratorium 4");

  delay(1000);
}