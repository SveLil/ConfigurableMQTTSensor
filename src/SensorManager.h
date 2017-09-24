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
  static int getSensorConfigInfoCount();
  static int getSensorConfigInfoCount(int index);
  static void registerSensor(const String& sensorType, SensorConfigInfo* configInfo, int configInfoCount, factoryFunctionType factoryFunction);
  static SensorConfigInfo* getSensorConfigInfo(int index);
  static String getSensorType(int index);
  static Sensor* createSensor(const SensorConfigurationStruct &sConfig);
private:
  static std::map<String,int> idIndexMap;
  static std::vector<String> idArray;
  static std::vector<SensorConfigInfo*> configInfoArray;
  static std::vector<int> sensorConfigInfoCount;
  static std::vector<factoryFunctionType> factoryFunctions;
};
#endif
