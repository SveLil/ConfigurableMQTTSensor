#ifndef MQTT_PUBLISHER
#define MQTT_PUBLISHER
#include <PubSubClient.h>
#include "ConfigurationStructs.h"
#include "Sensor.h"

class MQTTPublisher {
public:
  explicit MQTTPublisher(PubSubClient &client);
  void reconfigure(PubSubClient &client);
  void publish();
private:
  PubSubClient _client;
  String _boardName;
};
#endif
