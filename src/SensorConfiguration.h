#ifndef SENSOR_CONFIGURATION
#define SENSOR_CONFIGURATION

#include <map>
#include <ArduinoJson.h>
#include "ConfigurationStructs.h"

class SensorConfiguration {
public:
  explicit SensorConfiguration(const SensorConfigurationStruct configStruct) {
    sensorName = String(configStruct.sensorName);
    sensorType = String(configStruct.sensorType);
    StaticJsonBuffer<1024> jsonBuffer;
    JsonArray& jsonConfigArray = jsonBuffer.parseArray(configStruct.configString);
    std::map<String,String> configMap;
    for (auto value : jsonConfigArray) {
      JsonObject& vObject = value.as<JsonObject>();
      const char* cName = vObject["name"];
      const char* cValue = vObject["value"];
      configMap[String(cName)] = cValue;
    }
  };
  String sensorType;
  String sensorName;
  const std::map<String,const char*> configMap;
  const char* operator[] (const String& key) const {
    return configMap.find(key)->second;
  };
};
#endif
