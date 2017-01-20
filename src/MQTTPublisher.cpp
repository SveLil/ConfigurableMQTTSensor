#include "MQTTPublisher.h"
#include "BoardConfiguration.h"


MQTTPublisher::MQTTPublisher(PubSubClient &client) : _client(client) {}
void MQTTPublisher::reconfigure(PubSubClient &client) {
  _client = client;
}
void MQTTPublisher::publish() {
  Serial.println("Publish");
  BoardConfiguration& config = BoardConfiguration::getInstance();
  int sensorCount = config.getSensorCount();
  Serial.println("Publish data to mqtt for "+ String(sensorCount)+" sensors");
  Sensor** sensors = config.getSensors();
  Serial.println("got sensors");
  String boardName = config.getConfig().mqttConfig.boardName;
  for (int i=0; i<sensorCount; i++) {
    Serial.println("Get sensor no. "+ String(i));
    Sensor* current = sensors[i];
    if (current == NULL) {
      Serial.println("NULL sensor");
      Serial.flush();
    } else {
      Serial.println("Got sensor ");
      Serial.flush();
      int sCount = current->getSensorCount();
      Serial.println("Sub-Sensor count: "+ String(sCount));
      Serial.flush();
      for (int i=0; i<sCount;i++) {
        String name = current->getName(i);
        Serial.println("Sub-Sensor name: "+ name);
        Serial.flush();
        String value = current->getValue(i);
        Serial.println("Sub-Sensor value: "+ value);
        Serial.flush();
        String topic = boardName + "/" + name;
        Serial.println("publish!");
        Serial.flush();
        _client.publish(topic.c_str(), value.c_str());
      }
    }
  }
}
