#include <EEPROM.h>
#include <Arduino.h>

struct WiFiConfiguration {
  char ssid[32];
  char password[64];
};

struct MQTTConfiguration {
  char server[256];
  int port;
  bool useSSL;
  char user[64];
  char password[64];
  char sensorName[256];
};

struct ConfigurationStruct{
  int status;
  WiFiConfiguration wifiConfig;
  MQTTConfiguration mqttConfig;
};


class SensorConfiguration {
public:
  static void saveWifiConfiguration( String ssid, String password);
  static void  saveMQTTConfiguration(String s_server, int port, bool useSSL, String s_user,  String s_password, String s_sensorName);
  static bool isWifiConfigured();
  static bool connectToWifi();
  static ConfigurationStruct getConfig();

private:
  SensorConfiguration();
  SensorConfiguration(SensorConfiguration const&);  // Don't Implement
  void operator=(SensorConfiguration const&); // Don't implement
  static SensorConfiguration& getInstance();
  void save();
  ConfigurationStruct data;
};
