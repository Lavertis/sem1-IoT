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

float bmpT, bmpH;
int bmpP;
#include <Wire.h>

#include <Adafruit_BME280.h>
Adafruit_BME280 bme;

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

  if (!bme.begin())
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }
  else
  {
    Serial.println("BME280 sensor OK");
    bmpT = bme.readTemperature();
    bmpH = bme.readHumidity();
    bmpP = bme.readPressure() / 100.0;
    Serial.print("Temperature : ");
    Serial.println(bmpT);
    Serial.print("Humidity = ");
    Serial.println(bmpH);
    Serial.print("Pressure = ");
    Serial.println(bmpP);
  }
}

void loop(void)
{

  count++;
  bmpT = bme.readTemperature();
  bmpH = bme.readHumidity();
  bmpP = (int)bme.readPressure() / 100.0;

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
  tft.print("Sensor BME280");

  tft.setCursor(tft.width() * 0.3, tft.height() * 0.25, 2);
  tft.setTextColor(COLORT2);
  tft.print(String(bmpT) + " C");

  tft.setCursor(tft.width() * 0.3, tft.height() * 0.40, 2);
  tft.setTextColor(COLORT2);
  tft.print(String(bmpH) + " %");

  tft.setCursor(tft.width() * 0.275, tft.height() * 0.55, 2);
  tft.setTextColor(COLORT2);
  tft.print(String(bmpP) + " hPa");

  tft.setCursor(tft.width() * 0.125, tft.height() * 0.9, 2);
  tft.setTextColor(COLORT3);
  tft.print("Laboratorium 2");

  delay(2000);
  tft.setCursor(tft.width() * 0.3, tft.height() * 0.25, 2);
  tft.setTextColor(COLORT1);
  tft.print(String(bmpT) + " C");

  tft.setCursor(tft.width() * 0.3, tft.height() * 0.40, 2);
  tft.setTextColor(COLORT1);
  tft.print(String(bmpH) + " %");

  tft.setCursor(tft.width() * 0.2, tft.height() * 0.55, 2);
  tft.setTextColor(COLORT1);
  tft.print(String(bmpP) + " hPa");
}
