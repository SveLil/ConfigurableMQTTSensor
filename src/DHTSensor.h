#include "MQTTSensor.h"

class DHTSensor : public MQTTSensor {
public:
  DHTSensor(PubSubClient client, String boardName, SensorConfiguration sensorConfig);
private:
  int getSensorCount();
  String getValue(int index);
  String getName(int index);
};
