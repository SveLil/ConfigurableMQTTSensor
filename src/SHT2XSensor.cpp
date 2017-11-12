#include <Wire.h>
#include "SHT2XSensor.h"
#include "SensorManager.h"

SHT2XSensor::SHT2XSensor(const SensorConfiguration &sensorConfig): _sensorConfig(sensorConfig) {
  connected = sensor.begin();
  if (!connected) {
    Serial.println("Couldn't find sensor!");
  }
};

Sensor* createSHT2XSensor(const SensorConfiguration& sensorConfig) {
  return new SHT2XSensor(sensorConfig);
}

void SHT2XSensor::registerSensor() {
  SensorConfigInfo configInfo[0];
  SensorManager::getInstance().registerSensor("SHT2X", configInfo, 0, &createSHT2XSensor);
}

SensorConfigurationStruct SHT2XSensor::getConfig() {
  SensorConfigurationStruct config;
  _sensorConfig.sensorName.toCharArray(config.sensorName, 128);
  _sensorConfig.sensorType.toCharArray(config.sensorType,128);
  return config;
}

int SHT2XSensor::getSensorCount() {
  return 2;
}

String SHT2XSensor::getName(int index) {
  if (index == 0) {
    return "temperature";
  } else if (index == 1) {
    return "humidity";
  }
}

bool SHT2XSensor::getValue(const int index, String& value) {
  if (!connected) {
    Serial.println("Sensor not found!");
    return false;
  }
  Serial.println();
  float f_value = 0;
  if (index == 0) {
    f_value = sensor.readTemperature();
  } else {
    f_value = sensor.readHumidity();
  }
  if (isnan(f_value)) {
    Serial.println("Failed to read from Sensor!");
    return false;
  }
  value = String(f_value);

  Serial.print("Read: " + value + "(");
  Serial.print(f_value);
  Serial.println(")");
  return true;
}
