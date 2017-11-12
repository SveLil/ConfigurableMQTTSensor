#include "SensorManager.h"

SensorManager& SensorManager::getInstance() {
  static SensorManager me;
  return me;
}

int SensorManager::getSensorConfigInfoCount() {
  return configInfoArray.size();
}

int SensorManager::getSensorConfigInfoCount(int index) {
  return sensorConfigInfoCount[index];
}

SensorConfigInfo* SensorManager::getSensorConfigInfo(int index) {
  return configInfoArray[index];
}

String SensorManager::getSensorType(int index) {
 return idArray[index];
}

Sensor* SensorManager::createSensor(const SensorConfigurationStruct &sConfig) {
  int index = idIndexMap[String(sConfig.sensorType)];
  return (*factoryFunctions[index])(SensorConfiguration(sConfig));
}

void SensorManager::registerSensor(const String& sensorType, SensorConfigInfo* configInfo, int configInfoCount, factoryFunctionType factoryFunction) {
  int index = configInfoArray.size();
  idIndexMap[sensorType] = index;
  idArray.push_back(sensorType);
  configInfoArray.push_back(configInfo);
  sensorConfigInfoCount.push_back(configInfoCount);
  factoryFunctions.push_back(factoryFunction);
}
