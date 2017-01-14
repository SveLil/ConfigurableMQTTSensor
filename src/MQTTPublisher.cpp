#include "MQTTPublisher.h"
#include "BoardConfiguration.h"


MQTTPublisher::MQTTPublisher(PubSubClient &client) : _client(client) {}
void MQTTPublisher::reconfigure(PubSubClient &client) {
  _client = client;
}
void MQTTPublisher::publish() {
  BoardConfiguration& config = BoardConfiguration::getInstance();
  Sensor** sensors = config.getSensors();
  int sensorCount = config.getSensorCount();
  String boardName = config.getConfig().mqttConfig.boardName;
  for (int i=0; i<sensorCount; i++) {
    Sensor* current = sensors[i];
    for (int i=0; i<current->getSensorCount();i++) {
      String name = current->getName(i);
      String value = current->getValue(i);
      String topic = boardName + "/" + name;
      _client.publish(topic.c_str(), value.c_str());
    }

  }
}
