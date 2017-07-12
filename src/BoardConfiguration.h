#ifndef BOARD_CONFIGURATION
#define BOARD_CONFIGURATION
#include <EEPROM.h>
#include <Arduino.h>
#include <PubSubClient.h>
#include "ConfigurationStructs.h"
#include "Sensor.h"

class BoardConfiguration {
public:
  static BoardConfiguration& getInstance();
  void saveWifiConfiguration( const String& ssid, const String& password);
  void saveMQTTConfiguration(const String& s_server,const int port, const bool useSSL, const String& s_user,  const String& s_password, const String& s_baseTopic, const int readInterval);
  int saveSensorConfiguration( int sensorId, const SensorType& sensorType, const int pin,const String& sensorName);
  bool deleteSensorConfiguration(const int sensorId);
  bool connectToWifi();
  bool connectToMQTT(PubSubClient &client);
  ConfigurationStruct getConfig();
  Sensor** getSensors();
  SensorConfiguration getSensorConfig(const int sensorId);
  int getSensorCount();
private:
  BoardConfiguration();
  BoardConfiguration(BoardConfiguration const&);  // Don't Implement

  ConfigurationStruct data;
  SensorConfiguration sensorConfig[16];
  Sensor* *sensors;
  int createdSensorCount = 0;
  bool connectedToMQTT = false;
  bool sensorsInitialized = false;

  void operator=(BoardConfiguration const&); // Don't implement
  void save();
  void saveSensorConfiguration();
  void initSensors(int index);
  void debugPrintConfig(bool printData, bool printWifi, bool printMQTT);
  void debugPrintSensorConfig();
};
#endif
