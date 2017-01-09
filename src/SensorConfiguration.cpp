#include <SensorConfiguration.h>
#include <ESP8266WiFi.h>

SensorConfiguration::SensorConfiguration() {
  // Check status
  // 1: Not configured yet => Show Wifi SensorConfiguration page
  // 2: Wifi configured (and working?) => Connect to Wifi and show MQTT SensorConfiguration page
  // 3: MQTT confiugured => Normal startup. Keep Server running? Show status? Show SensorConfiguration page?
  EEPROM.begin(sizeof(data)+sizeof(data.status));
  EEPROM.get(0, data.status);
  if (data.status < 0 || data.status > 5) {
    data.status = 0;
    delay(500);
    EEPROM.put(0,data.status);
    data.wifiConfig.ssid[0] = 0;
    data.wifiConfig.password[0] = 0;
    EEPROM.put(sizeof(data.status),data.wifiConfig);
    EEPROM.commit();
  } else {
    EEPROM.get(sizeof(data.status),data.wifiConfig);
    Serial.println("Loaded");
    Serial.println("data.status : " + String(data.status));
    Serial.println("data.wifiConfig.ssid : " + String(data.wifiConfig.ssid));
    Serial.println("data.wifiConfig.password : " + String(data.wifiConfig.password));
    if (data.status > 1) {
      EEPROM.get(sizeof(data.status)+sizeof(data.wifiConfig),data.mqttConfig);
      Serial.println("data.mqttConfig.server : " + String(data.mqttConfig.server));
      Serial.println("data.mqttConfig.port : " + String(data.mqttConfig.port));
      Serial.println("data.mqttConfig.ssl : " + String(data.mqttConfig.useSSL));
      Serial.println("data.mqttConfig.user : " + String(data.mqttConfig.user));
      Serial.println("data.mqttConfig.password : " + String(data.mqttConfig.password));
      Serial.println("data.mqttConfig.sensorName : " + String(data.mqttConfig.sensorName));
    }
  }
}

// void updateEEPROM(int address,const T &t) {
//   &T existing;
//   EEPROM.get(address, existing);
//   if (existing == t) {
//     return;
//   }
//   EEPROM.put(address,t);
// }

void SensorConfiguration::save() {
  EEPROM.put(0,data.status);
  EEPROM.put(sizeof(data.status),data.wifiConfig);
  EEPROM.get(sizeof(data.status)+sizeof(data.wifiConfig),data.mqttConfig);
  EEPROM.commit();
  Serial.println("Saved");
  Serial.println("data.status : " + String(data.status));
  Serial.println("data.wifiConfig.ssid : " + String(data.wifiConfig.ssid));
  Serial.println("data.wifiConfig.password : " + String(data.wifiConfig.password));
  Serial.println("data.mqttConfig.server : " + String(data.mqttConfig.server));
  Serial.println("data.mqttConfig.port : " + String(data.mqttConfig.port));
  Serial.println("data.mqttConfig.ssl : " + String(data.mqttConfig.useSSL));
  Serial.println("data.mqttConfig.user : " + String(data.mqttConfig.user));
  Serial.println("data.mqttConfig.password : " + String(data.mqttConfig.password));
  Serial.println("data.mqttConfig.sensorName : " + String(data.mqttConfig.sensorName));
}

SensorConfiguration& SensorConfiguration::getInstance() {
  static SensorConfiguration SensorConfiguration;
  return SensorConfiguration;
}

void SensorConfiguration::saveWifiConfiguration(String s_ssid,  String s_password) {
  Serial.print("Saving WiFi configuration");
  SensorConfiguration& SensorConfiguration = getInstance();
  s_ssid.toCharArray(SensorConfiguration.data.wifiConfig.ssid,32);
  s_password.toCharArray(SensorConfiguration.data.wifiConfig.password,64);
  if (SensorConfiguration.data.status < 1) {
    SensorConfiguration.data.status = 1;
  }
  SensorConfiguration.save();
}

void SensorConfiguration::saveMQTTConfiguration(String s_server, int port, bool useSSL, String s_user,  String s_password, String s_sensorName) {
  Serial.print("Saving WiFi configuration");
  SensorConfiguration& SensorConfiguration = getInstance();
  s_server.toCharArray(SensorConfiguration.data.mqttConfig.server,256);
  SensorConfiguration.data.mqttConfig.port=port;
  SensorConfiguration.data.mqttConfig.useSSL=useSSL;
  s_user.toCharArray(SensorConfiguration.data.mqttConfig.user,64);
  s_password.toCharArray(SensorConfiguration.data.mqttConfig.password,64);
  s_sensorName.toCharArray(SensorConfiguration.data.mqttConfig.sensorName,256);
  if (SensorConfiguration.data.status < 2) {
    SensorConfiguration.data.status = 2;
  }
  SensorConfiguration.save();
}

bool SensorConfiguration::isWifiConfigured() {
  SensorConfiguration& SensorConfiguration = getInstance();
  return SensorConfiguration.data.status >= 1;
}

bool SensorConfiguration::connectToWifi() {
  if (!isWifiConfigured()) {
    Serial.println("Configuration not found");
    saveWifiConfiguration("H2G2", "758a52cda737fa24eaad3c1c793181427b05e357282ca634b65e9d7fdf053ee8");
  }
  SensorConfiguration& c = getInstance();
  Serial.println("Configuration found, connecting to " + String(c.data.wifiConfig.ssid));
  WiFi.begin(c.data.wifiConfig.ssid, c.data.wifiConfig.password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
}

ConfigurationStruct SensorConfiguration::getConfig() {
    SensorConfiguration& c = getInstance();
    return c.data;
}
