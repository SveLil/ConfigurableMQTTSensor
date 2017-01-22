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
    return "Temperature";
  } else {
    return "Humidity";
  }
}

String DHTSensor::getValue(int index) {
  float value = 0;
  if (index == 0) {
    value = dht.readTemperature();
  } else {
    value = dht.readHumidity();
  }
  if (isnan(value)) {
    Serial.println("Failed to read from DHT sensor!");
    return "";
  }
  return String(value);
}
