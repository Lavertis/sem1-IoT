#pragma once
#include <DS18B20.h> // https://github.com/matmunk/DS18B20

DS18B20 ds(5); // Pin 5
char addressString[20];

void initDS18B20()
{
    ds.selectNext();
    uint8_t address[8];
    ds.getAddress(address);
    for (int j = 0; j < sizeof(address); j++)
    {
        sprintf(&addressString[j * 2], "%02X", address[j]);
    }
}