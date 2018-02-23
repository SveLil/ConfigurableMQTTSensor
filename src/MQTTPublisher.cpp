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
  Serial.flush();
  lastMillis =  millis();
  Serial.println("Publish data to mqtt for "+ String(sensorCount)+" sensors");
  Serial.flush();
  std::vector<Sensor*> sensors = config.getSensors();
  if (sensors.size() != sensorCount) {
    Serial.println("Configured sensor count differs from vector size: "+ String(sensors.size()));
    Serial.flush();    
    return;
  }
  Serial.println("got sensors");
  Serial.flush();
  String baseTopic = config.getConfig().mqttConfig.baseTopic;
  for (int i=0; i<sensorCount; i++) {
    _client.loop();
    Serial.println("Get sensor no. "+ String(i));
    Sensor* current = sensors.at(0);
    if (current == NULL) {
      Serial.println("NULL sensor");
      Serial.flush();
    } else {
      Serial.println("Got sensor");
      Serial.flush();
      int sCount = current->getSensorCount();
      Serial.println("Sensor count: " + String(sCount));
      Serial.flush();
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
