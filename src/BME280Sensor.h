#ifndef BME280_SENSOR
#define BME280_SENSOR
#include <BME280I2C.h>
#include "Sensor.h"
#include "ConfigurationStructs.h"

class BME280Sensor : public Sensor {
public:
  explicit BME280Sensor(const SensorConfiguration &sensorConfig);
  int getSensorCount();
  bool getValue(int index, String& value);
  String getName(int index);
private:
  SensorConfiguration _sensorConfig;
  BME280I2C bme;
  float temp, hum, pres;
};
#endif
