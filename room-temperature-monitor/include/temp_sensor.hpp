#pragma once
#include <DS18B20.h> // https://github.com/matmunk/DS18B20
#include <vector>

struct TemperatureInfo
{
    float temperature;
    uint8_t resolution;
    String address;
};

void getAddressString(uint8_t address[8], String &addressString);

std::vector<TemperatureInfo> getTemperatures();
