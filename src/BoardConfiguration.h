#include <EEPROM.h>
#include <Arduino.h>
#include <PubSubClient.h>
#include "ConfigurationStructs.h"

class Sensor;

class BoardConfiguration {
public:
  static BoardConfiguration& getInstance();
  void saveWifiConfiguration( const String ssid, const String password);
  void saveMQTTConfiguration(const String s_server,const int port, const bool useSSL, const String s_user,  const String s_password, const String s_boardName);
  bool connectToWifi();
  bool connectToMQTT(PubSubClient client);
  ConfigurationStruct getConfig();
  Sensor** getSensors();
  int getSensorCount();
private:
  BoardConfiguration();
  BoardConfiguration(BoardConfiguration const&);  // Don't Implement

  ConfigurationStruct data;
  SensorConfiguration* sensorConfig;
  Sensor* *sensors;
  int createdSensorCount;

  void operator=(BoardConfiguration const&); // Don't implement
  void save();
  bool sensorsInitialized;
  void initSensors(PubSubClient client);
  void debugPrintConfig(bool printData, bool printWifi, bool printMQTT);
};
