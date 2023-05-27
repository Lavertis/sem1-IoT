#include "TemperatureSensor.hpp"

DS18B20 TemperatureSensor::ds{5};
std::vector<TemperatureInfo> TemperatureSensor::temperatures;

void TemperatureSensor::getAddressString(uint8_t address[8], String &addressString)
{
    char outBuffer[17];
    for (uint8_t i = 0; i < 8; ++i)
    {
        sprintf(&outBuffer[i * 2], "%02X", address[i]);
    }
    addressString = outBuffer;
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

String TemperatureSensor::getTemperaturesAsJson()
{
    StaticJsonDocument<512> doc;
    JsonArray tempArray = doc.to<JsonArray>();

    auto temperatures = TemperatureSensor::getTemperatures();
    for (TemperatureInfo &tempInfo : temperatures)
    {
        JsonObject tempObject = tempArray.createNestedObject();
        tempObject["temperature"] = tempInfo.temperature;
        tempObject["resolution"] = tempInfo.resolution;
        tempObject["address"] = tempInfo.address.c_str();
    }

    String json;
    serializeJson(tempArray, json);
    return json;
}