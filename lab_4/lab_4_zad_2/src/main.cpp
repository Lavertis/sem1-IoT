#include <Arduino.h>

// 30.07.2022
// dla nodemcu-32s
// współpraca z TFT ili9341
#include <SPI.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
#define COLORT1 TFT_BLACK
#define COLORT2 TFT_GREEN
#define COLORT3 TFT_RED
// Invoke library, pins defined in User_Setup.h

#include <DS18B20.h> //https://github.com/matmunk/DS18B20
DS18B20 ds(14);      // pin GPIO14
uint8_t address[8];

uint32_t count = 0;

void setup(void)
{
  Serial.begin(115200);
  delay(1000);
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(COLORT1);
  tft.setTextSize(2);
  Serial.println();
  Serial.print("TFT Init OK");

  ds.getAddress(address);
}

float temp1 = 1;
char addressStr[20];

void loop(void)
{

  count++;

  tft.drawLine(tft.width() * 0.65, 0, tft.width() * 0.65, tft.height() * 0.1, COLORT2);
  tft.fillRect(tft.width() * 0.655, 0, tft.width() * 0.35, tft.height() * 0.1, COLORT3);
  tft.setCursor(tft.width() * 0.7, tft.height() * 0.03, 1);
  tft.setTextColor(TFT_YELLOW);
  tft.print(String(count));

  tft.drawLine(0, tft.height() * 0.1, tft.width(), tft.height() * 0.1, COLORT2);
  tft.drawLine(0, tft.height() * 0.2, tft.width(), tft.height() * 0.2, COLORT2);
  tft.drawLine(0, tft.height() * 0.89, tft.width(), tft.height() * 0.89, COLORT2);

  tft.setCursor(tft.width() * 0.15, tft.height() * 0.13, 1);
  tft.setTextColor(COLORT2);
  tft.print("Sensor DS18B2");

  ds.select(address);

  float temp1_prev = temp1;
  temp1 = ds.getTempC();

  tft.setCursor(tft.width() * 0.3, tft.height() * 0.25, 2);
  tft.setTextColor(COLORT1);
  tft.print(String(temp1_prev) + " C");

  tft.setCursor(tft.width() * 0.1, tft.height() * 0.375, 1);
  tft.setTextColor(COLORT1);
  tft.print(addressStr);

  tft.setCursor(tft.width() * 0.3, tft.height() * 0.25, 2);
  tft.setTextColor(COLORT2);
  tft.print(String(temp1) + " C");

  for (int i = 0; i < sizeof(address); i++)
  {
    sprintf(&addressStr[i * 2], "%02X", address[i]);
  }

  tft.setCursor(tft.width() * 0.1, tft.height() * 0.375, 1);
  tft.setTextColor(COLORT2);
  tft.print(addressStr);

  tft.setCursor(tft.width() * 0.125, tft.height() * 0.9, 2);
  tft.setTextColor(COLORT3);
  tft.print("Laboratorium 4");

  delay(1000);
}
