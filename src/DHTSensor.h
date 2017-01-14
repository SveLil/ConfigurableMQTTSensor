#ifndef DHT_SENSOR
#define DHT_SENSOR
#include "DHT.h"
#include "Sensor.h"
#include "ConfigurationStructs.h"

class DHTSensor : public Sensor {
public:
  explicit DHTSensor(SensorConfiguration &sensorConfig);
  int getSensorCount();
  String getValue(int index);
  String getName(int index);
private:
  SensorConfiguration _sensorConfig;
  DHT dht;
};
#endif
