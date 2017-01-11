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
  char boardName[256];
};

enum SensorType {
  DHT22,
  ANALOG,
  DIGITAL,
};

struct SingleSensorConfiguration {
  char sensorName[256] ;
  int pin;
  SensorType sensorType;
};

struct ConfigurationStruct {
  int status;
  int sensorCount;
  WiFiConfiguration wifiConfig;
  MQTTConfiguration mqttConfig;
};


class SensorConfiguration {
public:
  static void saveWifiConfiguration( const String ssid, const String password);
  static void saveMQTTConfiguration(const String s_server,const int port, const bool useSSL, const String s_user,  const String s_password, const String s_boardName);
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
