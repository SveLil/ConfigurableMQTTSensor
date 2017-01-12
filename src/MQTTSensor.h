#include <PubSubClient.h>
#include "ConfigurationStructs.h"
#include "Sensor.h"

class MQTTSensor : public Sensor {
public:
  MQTTSensor(PubSubClient client, String boardName, SensorConfiguration sensorConfig);
  void reconfigure(PubSubClient client);
  void reconfigure(String boardName);
  void reconfigure(SensorConfiguration sensorConfig);
  void read();
private:
  PubSubClient client;
  String boardName;
  SensorConfiguration sensorConfig;
  virtual int getSensorCount()=0;
  virtual String getValue(int index)=0;
  virtual String getName(int index)=0;
};
