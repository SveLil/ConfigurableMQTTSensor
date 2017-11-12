#include "MQTTPublisher.h"
#include "BoardConfiguration.h"
#include <ESP8266WiFi.h>

MQTTPublisher::MQTTPublisher(PubSubClient &client) : _client(client) {}

void MQTTPublisher::publish() {
  BoardConfiguration& config = BoardConfiguration::getInstance();
  config.connectToMQTT(_client);
  int sensorCount = config.getSensorCount();
  if (sensorCount == 0) {
    return;
  }

  if (!firstRun) {
    unsigned long currentMillis = millis();
    unsigned long interval = config.getConfig().mqttConfig.readInterval;
    if (currentMillis - lastMillis < interval * 60000) {
      return;
    }
  } else {
    firstRun = false;
  }
  Serial.println("Publish");
  lastMillis =  millis();
  Serial.println("Publish data to mqtt for "+ String(sensorCount)+" sensors");
  //Sensor** sensors = config.getSensors();
  std::vector<Sensor*> sensors = config.getSensors();
  Serial.println("got sensors");
  String baseTopic = config.getConfig().mqttConfig.baseTopic;
  for (int i=0; i<sensorCount; i++) {
    _client.loop();
    Serial.println("Get sensor no. "+ String(i));
    Sensor* current = sensors[i];
    if (current == NULL) {
      Serial.println("NULL sensor");
      Serial.flush();
    } else {
      int sCount = current->getSensorCount();
      String baseName = String(config.getSensorConfig(i).sensorName);
      if (baseTopic.length() > 0) {
        baseTopic = baseTopic + "/" + String(baseName);
      } else {
        baseTopic = String(baseName);
      }
      for (int i=0; i<sCount;i++) {
        String name = current->getName(i);
        String value;
        bool success = current->getValue(i,value);
        if (success) {
          String topic = baseTopic + "/" + name;
          Serial.println("publish '"+ value+"' to: " + topic);
          Serial.flush();
          bool success = _client.publish(topic.c_str(), value.c_str(), true);
          if (!success) {
            Serial.println("Publish failed");
            Serial.flush();
          }
        }
      }
    }
  }
}
