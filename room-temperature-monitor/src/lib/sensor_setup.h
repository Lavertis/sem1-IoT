#pragma once
#include <DS18B20.h> // https://github.com/matmunk/DS18B20
#include <vector>

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

std::vector<float> getTemperatures()
{
    std::vector<float> temperatures;
    while (ds.selectNext())
    {
        ds.setResolution(12);
        float temp = ds.getTempC();
        temperatures.push_back(temp);
    }
    // add some random values
    temperatures.push_back(20.0);
    temperatures.push_back(21.0);
    temperatures.push_back(22.0);
    temperatures.push_back(23.0);
    temperatures.push_back(24.0);
    temperatures.push_back(25.0);
    temperatures.push_back(26.0);
    return temperatures;
}