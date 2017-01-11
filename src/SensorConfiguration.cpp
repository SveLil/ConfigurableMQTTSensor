#include <SensorConfiguration.h>
#include <ESP8266WiFi.h>

const char testString[] = "OK3";

SensorConfiguration::SensorConfiguration() {
  // Check status
  // 1: Not configured yet => Show Wifi SensorConfiguration page
  // 2: Wifi configured (and working?) => Connect to Wifi and show MQTT SensorConfiguration page
  // 3: MQTT confiugured => Normal startup. Keep Server running? Show status? Show SensorConfiguration page?
  char s[4];
  int loc = sizeof(s);
  Serial.println("SizeOf: "+String(loc)+"+" + String(sizeof(data)));
  EEPROM.begin(4096);
  EEPROM.get(0, s);
  if (String(s) != String(testString)) {
    Serial.println("Clean, because: " + String(s));
    data.wifiConfig.ssid[0] = 0;
    data.wifiConfig.password[0] = 0;
    delay(500);
    loc = sizeof(s);
    EEPROM.put(0, testString);
    EEPROM.put(loc,data);
    EEPROM.commit();
    delay(500);
  } else {
    EEPROM.get(loc, data);
    loc += sizeof(data.wifiConfig);
    Serial.println("Loaded");
    Serial.println("data.status : " + String(data.status));
    Serial.println("data.wifiConfig.ssid : " + String(data.wifiConfig.ssid));
    Serial.println("data.wifiConfig.password : " + String(data.wifiConfig.password));
    if (data.status > 1) {
      loc += sizeof(data.mqttConfig);
      Serial.println("data.mqttConfig.server : " + String(data.mqttConfig.server));
      Serial.println("data.mqttConfig.port : " + String(data.mqttConfig.port));
      Serial.println("data.mqttConfig.ssl : " + String(data.mqttConfig.useSSL));
      Serial.println("data.mqttConfig.user : " + String(data.mqttConfig.user));
      Serial.println("data.mqttConfig.password : " + String(data.mqttConfig.password));
      Serial.println("data.mqttConfig.boardName : " + String(data.mqttConfig.boardName));
    }
  }
}

void SensorConfiguration::save() {
  int loc = sizeof(testString);
  EEPROM.put(loc,data);
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
  Serial.println("data.mqttConfig.boardName : " + String(data.mqttConfig.boardName));
}

SensorConfiguration& SensorConfiguration::getInstance() {
  static SensorConfiguration SensorConfiguration;
  return SensorConfiguration;
}

void SensorConfiguration::saveWifiConfiguration(String s_ssid,  String s_password) {
  Serial.println("Saving WiFi configuration");
  SensorConfiguration& SensorConfiguration = getInstance();
  s_ssid.toCharArray(SensorConfiguration.data.wifiConfig.ssid,32);
  s_password.toCharArray(SensorConfiguration.data.wifiConfig.password,64);
  if (SensorConfiguration.data.status < 1) {
    SensorConfiguration.data.status = 1;
    SensorConfiguration.data.mqttConfig.server[0] = 0;
    SensorConfiguration.data.mqttConfig.user[0] = 0;
    SensorConfiguration.data.mqttConfig.password[0] = 0;
    SensorConfiguration.data.mqttConfig.boardName[0] = 0;
    SensorConfiguration.data.mqttConfig.useSSL = false;
    SensorConfiguration.data.mqttConfig.port = 0;
  }
  SensorConfiguration.save();
}

void SensorConfiguration::saveMQTTConfiguration(String s_server, int port, bool useSSL, String s_user,  String s_password, String s_boardName) {
  Serial.println("Saving MQTT configuration");
  SensorConfiguration& SensorConfiguration = getInstance();
  s_server.toCharArray(SensorConfiguration.data.mqttConfig.server,256);
  SensorConfiguration.data.mqttConfig.port=port;
  SensorConfiguration.data.mqttConfig.useSSL=useSSL;
  s_user.toCharArray(SensorConfiguration.data.mqttConfig.user,64);
  s_password.toCharArray(SensorConfiguration.data.mqttConfig.password,64);
  s_boardName.toCharArray(SensorConfiguration.data.mqttConfig.boardName,256);
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
    saveWifiConfiguration("H2G2", "2edelw6wlan");
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
