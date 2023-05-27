#pragma once
#include <DS18B20.h>
#include <ArduinoJson.h>
#include <vector>

struct TemperatureInfo
{
    float temperature;
    uint8_t resolution;
    String address;
};

class TemperatureSensor
{
public:
    static void update();
    static std::vector<TemperatureInfo> getTemperatures();
    static String getTemperaturesAsJson();

private:
    static void getAddressString(uint8_t address[8], String &addressString);

    static DS18B20 ds;
    static std::vector<TemperatureInfo> temperatures;
    static constexpr uint8_t resolution = 12;
};
