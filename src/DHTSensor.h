#ifndef DHT_SENSOR
#define DHT_SENSOR
#include <DHT.h>
#include "Sensor.h"
#include "SensorConfiguration.h"

class DHTSensor : public Sensor {
public:
  explicit DHTSensor(const SensorConfiguration &sensorConfig);
  int getSensorCount();
  bool getValue(int index, String& value);
  String getName(int index);
  static void registerSensor();
  SensorConfigurationStruct getConfig();
private:
  SensorConfiguration _sensorConfig;
  DHT dht;
};
#endif
