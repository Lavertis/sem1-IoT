#include "TemperatureSensor.hpp"

DS18B20 TemperatureSensor::ds{5};
std::vector<TemperatureInfo> TemperatureSensor::temperatures;

void TemperatureSensor::getAddressString(uint8_t address[8], String &addressString)
{
    for (uint8_t j = 0; j < 8; j++)
    {
        char buff[3];
        sprintf(buff, "%02X", address[j]);
        addressString += buff;
    }
}

void TemperatureSensor::update()
{
    std::vector<TemperatureInfo> tempTemperatures;

    while (ds.selectNext())
    {
        uint8_t address[8];
        ds.getAddress(address);
        String addressString;
        getAddressString(address, addressString);

        constexpr uint8_t resolution = 9;
        ds.setResolution(resolution);

        TemperatureInfo tempInfo;
        tempInfo.temperature = ds.getTempC();
        tempInfo.resolution = resolution;
        tempInfo.address = addressString;

        tempTemperatures.push_back(tempInfo);
    }

    TemperatureSensor::temperatures = tempTemperatures;
}

std::vector<TemperatureInfo> TemperatureSensor::getTemperatures()
{
    return TemperatureSensor::temperatures;
}
