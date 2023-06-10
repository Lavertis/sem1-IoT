#include "TemperatureSensor.hpp"

DS18B20 TemperatureSensor::ds{5};
std::vector<TemperatureInfo> TemperatureSensor::temperatures;
std::mutex temperatures_mutex;
uint8_t TemperatureSensor::resolution = 12;

void TemperatureSensor::setResolution(uint8_t resolution)
{
    std::lock_guard<std::mutex> lock(temperatures_mutex);
    TemperatureSensor::resolution = resolution;
}

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

    std::lock_guard<std::mutex> lock(temperatures_mutex);
    temperatures = tempTemperatures;
}

std::vector<TemperatureInfo> TemperatureSensor::getTemperatures()
{
    std::lock_guard<std::mutex> lock(temperatures_mutex);
    return temperatures;
}

String TemperatureSensor::getTemperaturesAsJson()
{
    StaticJsonDocument<512> doc;
    JsonArray tempArray = doc.to<JsonArray>();

    std::vector<TemperatureInfo> localTemperatures;
    {
        std::lock_guard<std::mutex> lock(temperatures_mutex);
        localTemperatures = temperatures;
    }

    for (TemperatureInfo &tempInfo : localTemperatures)
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
