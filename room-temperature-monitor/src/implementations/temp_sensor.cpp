#include "temp_sensor.hpp"

DS18B20 ds(5);

void getAddressString(uint8_t address[8], String &addressString)
{
    for (uint8_t j = 0; j < 8; j++)
    {
        char buff[3];
        sprintf(buff, "%02X", address[j]);
        addressString += buff;
    }
}

std::vector<TemperatureInfo> getTemperatures()
{
    std::vector<TemperatureInfo> temperatures;
    uint8_t address[8];
    String addressString;

    while (ds.selectNext())
    {
        ds.getAddress(address);
        addressString = "";
        getAddressString(address, addressString);

        uint8_t resolution = 10;
        ds.setResolution(resolution);
        float temp = ds.getTempC();

        TemperatureInfo tempInfo;
        tempInfo.temperature = temp;
        tempInfo.resolution = resolution;
        tempInfo.address = addressString;

        temperatures.push_back(tempInfo);
    }

    // add some random values
    temperatures.push_back({20.0, 10, "12345678"});
    temperatures.push_back({23.0, 10, "12345678"});
    temperatures.push_back({26.0, 10, "12345678"});
    temperatures.push_back({29.0, 10, "12345678"});
    temperatures.push_back({32.0, 10, "12345678"});

    return temperatures;
}
