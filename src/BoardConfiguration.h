
#ifndef BOARD_CONFIGURATION
#define BOARD_CONFIGURATION
#include <EEPROM.h>
#include <Arduino.h>
#include <map>
#include <PubSubClient.h>
#include "ConfigurationStructs.h"
#include "Sensor.h"
#include "SensorConfiguration.h"

class BoardConfiguration {
public:
  static BoardConfiguration& getInstance();
  void saveWifiConfiguration( const String& ssid, const String& password, const bool enableAP);
  void saveMQTTConfiguration(const String& s_server,const int port, const bool useSSL, const String& s_user,  const String& s_password, const String& s_baseTopic, const int readInterval, const bool enableDeepSleep);
  int saveSensorConfigurationStruct( int sensorId, const String& sensorType, const String& configString, const String& sensorName);
  bool deleteSensorConfigurationStruct(const int sensorId);
  bool connectToWifi();
  bool connectToMQTT(PubSubClient &client);
  ConfigurationStruct getConfig();
  Sensor** getSensors();
  SensorConfigurationStruct getSensorConfig(const int sensorId);
  int getSensorCount();
private:
  BoardConfiguration();
  BoardConfiguration(BoardConfiguration const&);  // Don't Implement

  ConfigurationStruct data;
  SensorConfigurationStruct sensorConfig[16];
  Sensor* *sensors;
  int createdSensorCount = 0;
  bool connectedToMQTT = false;
  bool sensorsInitialized = false;

  void operator=(BoardConfiguration const&); // Don't implement
  void save();
  void saveSensorConfigurationStruct();
  void initSensors(int index);
  void debugPrintConfig(bool printData, bool printWifi, bool printMQTT);
  void debugPrintSensorConfig();
  void updateConfig01To02();
  void updateConfig02To03();
  void wipe();
};
#endif
