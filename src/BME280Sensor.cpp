#include "BME280Sensor.h"
#include <BME280I2C.h>
#include <Wire.h>
#include <SPI.h>

BME280Sensor::BME280Sensor(const SensorConfiguration &sensorConfig): _sensorConfig(sensorConfig) {//, dht(sensorConfig.pin, DHT22) {
  bme.begin();
  hum = pres = temp = NAN;
};

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
    bme.read(pres, temp, hum, true, 0x1);
    f_value = temp;
  } else if (index == 1) {
    f_value = hum;
  } else {
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
