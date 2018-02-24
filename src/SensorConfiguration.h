#ifndef SENSOR_CONFIGURATION
#define SENSOR_CONFIGURATION

#include <map>
#include <ArduinoJson.h>
#include "ConfigurationStructs.h"

class SensorConfiguration {
public:
  String sensorType;
  String sensorName;
  std::map<String,const char*> configMap;
  explicit SensorConfiguration(const SensorConfigurationStruct &configStruct) {
    sensorName = String(configStruct.sensorName);
    sensorType = String(configStruct.sensorType);
    DynamicJsonBuffer jsonBuffer(CONFIG_STRING_SIZE);
    JsonArray& jsonConfigArray = jsonBuffer.parseArray(configStruct.configString);
    for (auto value : jsonConfigArray) {
      JsonObject& vObject = value.as<JsonObject>();
      const char* cName = vObject["name"];
      const char* cValue = vObject["value"];
      configMap[String(cName)] = cValue;
    }
  };
  const char* operator[] (const String& key) const {
    return configMap.find(key)->second;
  };
  SensorConfigurationStruct toConfigStruct() {
    SensorConfigurationStruct config;
    sensorName.toCharArray(config.sensorName, 128);
    sensorType.toCharArray(config.sensorType,128);
    DynamicJsonBuffer jsonBuffer(CONFIG_STRING_SIZE);
    JsonArray& root = jsonBuffer.createArray();

  	for (std::map<String, const char*>::iterator it = configMap.begin(); it != configMap.end(); it++)
  	{
  		String key = it->first;
  		const char* value = it->second;
      JsonObject& o = jsonBuffer.createObject();
      o["name"]=key.c_str();
      o["value"]=value;
      root.add(o);
  	}

    root.printTo(config.configString, root.measureLength() + 1);
    return config;
  }
};
#endif
