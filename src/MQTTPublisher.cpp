#include "MQTTPublisher.h"
#include "BoardConfiguration.h"

MQTTPublisher::MQTTPublisher(PubSubClient &client) : _client(client) {}

void MQTTPublisher::publish() {
  BoardConfiguration& config = BoardConfiguration::getInstance();
  if (!_client.loop() || _client.state()) {
    int mqttState = _client.state();
    if (mqttState == MQTT_DISCONNECTED) {
      //config.connectToMQTT(_client);
    } else {
      String error = "";
      switch (mqttState) {
        case MQTT_CONNECTION_TIMEOUT : error = "MQTT_CONNECTION_TIMEOUT"; break;
        case MQTT_CONNECTION_LOST : error = "MQTT_CONNECTION_LOST"; break;
        case MQTT_CONNECT_FAILED : error = "MQTT_CONNECT_FAILED"; break;
        case MQTT_DISCONNECTED : error = "MQTT_DISCONNECTED"; break;
        case MQTT_CONNECT_BAD_PROTOCOL : error = "MQTT_CONNECT_BAD_PROTOCOL"; break;
        case MQTT_CONNECT_BAD_CLIENT_ID : error = "MQTT_CONNECT_BAD_CLIENT_ID"; break;
        case MQTT_CONNECT_UNAVAILABLE : error = "MQTT_CONNECT_UNAVAILABLE"; break;
        case MQTT_CONNECT_BAD_CREDENTIALS : error = "MQTT_CONNECT_BAD_CREDENTIALS"; break;
        case MQTT_CONNECT_UNAUTHORIZED : error = "MQTT_CONNECT_UNAUTHORIZED"; break;
      }
      Serial.println("MQTT: " + error);
      return;
    }
  }
  int sensorCount = config.getSensorCount();
  if (sensorCount == 0) {
    Serial.println("No sensors, returning");
    return;
  }

  unsigned long currentMillis = millis();
  unsigned long interval = config.getConfig().mqttConfig.readInterval;
  if (currentMillis - lastMillis < interval * 60000) {
    return;
  }
  Serial.println("Publish");
  lastMillis = currentMillis;
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
        String value;
        bool success = current->getValue(i,value);
        if (success) {
          Serial.println("Sub-Sensor value: "+ value);
          Serial.flush();
          String topic = boardName + "/" + name;
          Serial.println("publish to: " + topic);
          Serial.flush();
          bool success = _client.publish(topic.c_str(), value.c_str());
          if (!success) {
            Serial.println("Publish failed");
          }
        }
      }
    }
  }
}
