#ifndef CONFIG_STRUCTS
#define CONFIG_STRUCTS

enum SensorType01 {
  NO_SENSOR,
  DHT22_COMPATIBLE,
  SIMPLE_ANALOG,
  SIMPLE_DIGITAL,
  BME280
};

struct WiFiConfiguration {
  char ssid[32];
  char password[64];
  bool enableAP;
};

struct MQTTConfiguration {
  char server[256];
  int port;
  bool useSSL;
  char user[64];
  char password[64];
  char baseTopic[256];
  int readInterval;
};

const int CONFIG_STRING_SIZE = 512;
struct SensorConfigurationStruct {
  char configString[CONFIG_STRING_SIZE];
  char sensorType[128];
  char sensorName[128];
};

struct ConfigurationStruct {
  int status;
  int sensorCount;
  bool enableDeepSleep;
  WiFiConfiguration wifiConfig;
  MQTTConfiguration mqttConfig;
};

struct WiFiConfiguration01 {
  char ssid[32];
  char password[64];
};
struct ConfigurationStruct01 {
  int status;
  int sensorCount;
  WiFiConfiguration01 wifiConfig;
  MQTTConfiguration mqttConfig;
};

struct SensorConfigurationStruct01 {
  int pin;
  SensorType01 sensorType;
  char sensorName[256];
};
#endif
