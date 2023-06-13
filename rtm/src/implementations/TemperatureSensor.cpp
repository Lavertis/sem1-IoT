#include "TemperatureSensor.hpp"

DS18B20 TemperatureSensor::ds{5};
std::vector<TemperatureInfo> TemperatureSensor::temperatures;
uint8_t TemperatureSensor::resolution = 12;
String TemperatureSensor::sensorAddresses[] = {"28686BE50E000021", "2804A20F0D0000F8", "28D452E70E00002B", "28A6ED0E0D0000C3", "2885819D0E000091"};
String TemperatureSensor::sensorNames[] = {"Kuchnia", "Salon", "Sypialnia", "Łazienka", "Pokój gościnny"};

std::mutex temperatures_mutex;

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
        tempInfo.name = getNameForAddress(addressString);

        tempTemperatures.push_back(tempInfo);
    }

    std::lock_guard<std::mutex> lock(temperatures_mutex);
    temperatures = tempTemperatures;
}

String TemperatureSensor::getNameForAddress(const String& address)
{
    for (size_t i = 0; i < sizeof(sensorAddresses) / sizeof(sensorAddresses[0]); i++)
    {
        if (address == sensorAddresses[i])
        {
            return sensorNames[i];
        }
    }
    return "";
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
        tempObject["name"] = tempInfo.name.c_str();
    }

    String json;
    serializeJson(tempArray, json);
    return json;
}
