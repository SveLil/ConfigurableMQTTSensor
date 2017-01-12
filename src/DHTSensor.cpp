#include "DHTSensor.h"
#include <DHT.h>

String DHTSensor::getName(int index) {
  if (index == 0) {
    return "Temperature";
  } else {
    return "Humidity";
  }
}

int DHTSensor::getSensorCount() {
  return 2;
}

String DHTSensor::getValue(int index) {
  if (index == 0) {
    SimpleDHT::
    return "Temperature";
  } else {
    return "Humidity";
  }
}
