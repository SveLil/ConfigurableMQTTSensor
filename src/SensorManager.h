#ifndef SENSOR_MANAGER
#define SENSOR_MANAGER
#include <vector>
#include <map>
#include "ConfigurationStructs.h"
#include "Sensor.h"
#include "SensorConfiguration.h"

typedef Sensor* (*factoryFunctionType) (const SensorConfiguration&);

enum ConfigType {
  STRING,
  BOOLEAN,
  INTEGER,
  FLOAT
};

static const char* ConfigTypeNames[] = {"STRING", "BOOLEAN", "INTEGER", "FLOAT"};

struct SensorConfigInfo {
  String configName;
  ConfigType configType;
};

class SensorManager {
public:
  static SensorManager& getInstance();
  int getSensorConfigInfoCount();
  int getSensorConfigInfoCount(int index);
  void registerSensor(const String& sensorType, SensorConfigInfo* configInfo, int configInfoCount, factoryFunctionType factoryFunction);
  SensorConfigInfo* getSensorConfigInfo(int index);
  String getSensorType(int index);
  Sensor* createSensor(const SensorConfigurationStruct &sConfig);
private:
  std::map<String,int> idIndexMap;
  std::vector<SensorConfigInfo*> configInfoArray;
  std::vector<int> sensorConfigInfoCount;
  std::vector<factoryFunctionType> factoryFunctions;
  std::vector<String> idArray;
};
#endif
