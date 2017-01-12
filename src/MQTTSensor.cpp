#include "MQTTSensor.h"

MQTTSensor::MQTTSensor(PubSubClient client, String boardName, SensorConfiguration sensorConfig) {
  this->client = client;
  this->boardName = boardName;
  this->sensorConfig = sensorConfig;
}
void MQTTSensor::reconfigure(PubSubClient client) {
  this->client = client;
}
void MQTTSensor::reconfigure(String boardName) {
  this->boardName = boardName;
}
void MQTTSensor::reconfigure(SensorConfiguration sensorConfig) {
  this->sensorConfig = sensorConfig;
}
void MQTTSensor::read() {
  for (int i=0; i<getSensorCount();i++) {
    String name = getName(i);
    String value = getValue(i);
    String topic = boardName + "/" + name;
    client.publish(topic.c_str(), value.c_str());
  }
}
