#include <BME280I2C.h>
#include <EnvironmentCalculations.h>
#include <Wire.h>
#include <SPI.h>
#include "BME280Sensor.h"
#include "SensorManager.h"
#include "Sensor.h"

const char LOG_SEA_LEVEL_KEY[] = "Log sea level pressure";
const char ALTITUDE_KEY[] = "Altitude";

BME280Sensor::BME280Sensor(const SensorConfiguration &sensorConfig): _sensorConfig(sensorConfig) {
  bme.begin();
  hum = pres = temp = NAN;
  logSeaLevel = String(_sensorConfig[LOG_SEA_LEVEL_KEY]) == "true";
  alt = String(_sensorConfig[ALTITUDE_KEY]).toFloat();
};

Sensor* createBME280Sensor(const SensorConfiguration& sensorConfig) {
  return new BME280Sensor(sensorConfig);
}

void BME280Sensor::registerSensor() {
  SensorConfigInfo *configInfo = new SensorConfigInfo[2];
  strncpy(configInfo[0].configName, LOG_SEA_LEVEL_KEY, sizeof(LOG_SEA_LEVEL_KEY));
  configInfo[0].configType = BOOLEAN;
  strncpy(configInfo[1].configName, ALTITUDE_KEY, sizeof(ALTITUDE_KEY));
  configInfo[1].configType = INTEGER;
  SensorManager::getInstance().registerSensor("BME280", configInfo, 2, &createBME280Sensor);
}

SensorConfigurationStruct BME280Sensor::getConfig() {
  SensorConfigurationStruct config;
  _sensorConfig.sensorName.toCharArray(config.sensorName, 128);
  _sensorConfig.sensorType.toCharArray(config.sensorType,128);
  return config;
}

int BME280Sensor::getSensorCount() {
  return 3;
}

String BME280Sensor::getName(int index) {
  if (index == 0) {
    return "temperature";
  } else if (index == 1) {
    return "humidity";
  } else {
    return "pressure";
  }
}

bool BME280Sensor::getValue(const int index, String& value) {
  float f_value = 0;
  if (index == 0) {
    bme.read(pres, temp, hum, BME280::TempUnit::TempUnit_Celsius, BME280::PresUnit_hPa);
    f_value = temp;
  } else if (index == 1) {
    f_value = hum;
  } else {
    if (logSeaLevel) {
      pres =  EnvironmentCalculations::EquivalentSeaLevelPressure(alt, temp, pres);
    }
    f_value = pres;
  }
  if (isnan(f_value)) {
    Serial.println("Failed to read from BME280 sensor!");
    return false;
  }
  value = String(f_value);

  Serial.print("Read: " + value + "(");
  Serial.print(f_value);
  Serial.println(")");
  return true;
}
