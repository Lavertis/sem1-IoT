#include <Arduino.h>

#include <DS18B20.h> // https://github.com/matmunk/DS18B20
DS18B20 ds(5);       // pin GPIO5

uint8_t address[8];
char addressString[20];

void setup(void)
{
  Serial.begin(115200);
  delay(1000);

  ds.selectNext();
  ds.getAddress(address);
  for (int j = 0; j < sizeof(address); j++)
  {
    sprintf(&addressString[j * 2], "%02X", address[j]);
  }

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