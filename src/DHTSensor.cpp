#include "DHTSensor.h"
#include <DHT.h>
#include <Adafruit_Sensor.h>

DHTSensor::DHTSensor(const SensorConfiguration &sensorConfig) : _sensorConfig(sensorConfig), dht(sensorConfig.pin, DHT22) {
  dht.begin();
};

int DHTSensor::getSensorCount() {
  return 2;
}

String DHTSensor::getName(int index) {
  if (index == 0) {
    return "temperature";
  } else {
    return "humidity";
  }
}

bool DHTSensor::getValue(const int index, String& value) {
  float f_value = 0;
  if (index == 0) {
    f_value = dht.readTemperature();
  } else {
    f_value = dht.readHumidity();
  }
  if (isnan(f_value)) {
    Serial.println("Failed to read from DHT sensor!");
    return false;
  }
  value = String(f_value);
  Serial.println("Read: " + value);
  return true;
}
