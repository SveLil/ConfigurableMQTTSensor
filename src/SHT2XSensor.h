#ifndef SHT2XSensor
#define STH2XSensor
#include <Adafruit_HTU21DF.h>
#include "Sensor.h"
#include "SensorConfiguration.h"

class SHT2XSensor : public Sensor {
public:
  explicit SHT2XSensor(const SensorConfiguration &sensorConfig);
  int getSensorCount();
  bool getValue(int index, String& value);
  String getName(int index);
  static void registerSensor();
private:
  SensorConfiguration _sensorConfig;
  Adafruit_HTU21DF sensor;
  bool connected;
};
#endif
