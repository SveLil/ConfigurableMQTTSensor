#ifndef DHT_SENSOR
#define DHT_SENSOR
#include "DHT.h"
#include "Sensor.h"
#include "ConfigurationStructs.h"

class DHTSensor : public Sensor {
public:
  explicit DHTSensor(const SensorConfiguration &sensorConfig);
  int getSensorCount();
  bool getValue(int index, String& value);
  String getName(int index);
private:
  SensorConfiguration _sensorConfig;
  DHT dht;
};
#endif
