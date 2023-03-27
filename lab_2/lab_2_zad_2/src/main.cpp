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
}

void loop(void)
{
  tft.setTextColor(COLORT2);
  tft.drawLine(tft.width() * 0.65, 0, tft.width() * 0.65, tft.height() * 0.1, COLORT2);
  tft.drawLine(0, tft.height() * 0.1, tft.width(), tft.height() * 0.1, COLORT2);
  tft.drawLine(0, tft.height() * 0.2, tft.width(), tft.height() * 0.2, COLORT2);
  tft.drawLine(tft.width() / 2, tft.height() * 0.1, tft.width() / 2, tft.height() * 0.89, COLORT2);
  tft.drawLine(0, tft.height() * 0.89, tft.width(), tft.height() * 0.89, COLORT2);

  tft.setCursor(tft.width() * 0.05, tft.height() * 0.13, 1);
  tft.setTextColor(COLORT2);
  tft.print("Pomiar 1");

  tft.setCursor(tft.width() * 0.05, tft.height() * 0.25, 1);
  tft.setTextColor(COLORT2);
  tft.print("25.55 C");

  tft.setCursor(tft.width() * 0.05, tft.height() * 0.35, 1);
  tft.setTextColor(COLORT2);
  tft.print("56.76 %");

  tft.setCursor(tft.width() * 0.05, tft.height() * 0.45, 1);
  tft.setTextColor(COLORT2);
  tft.print("1013 hPa");

  tft.setCursor(tft.width() * 0.55, tft.height() * 0.13, 1);
  tft.setTextColor(COLORT2);
  tft.print("Pomiar 2");

  tft.setCursor(tft.width() * 0.125, tft.height() * 0.9, 2);
  tft.setTextColor(COLORT3);
  tft.print("Laboratorium 2");
}