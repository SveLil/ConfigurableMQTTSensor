#include <Esp.h>
#include <ESP8266WiFi.h>
#include <map>
#include "BoardConfiguration.h"
#include "SensorManager.h"

const char currentVersion[] = "V0.3";
const char v02String[] = "V0.2";
const char v01String[] = "V0.1";

WiFiClientSecure secureNet;
WiFiClient net;

BoardConfiguration::BoardConfiguration() {
  // Check status
  // 1: Not configured yet => Show Wifi BoardConfiguration page
  // 2: Wifi configured (and working?) => Connect to Wifi and show MQTT BoardConfiguration page
  // 3: MQTT confiugured => Normal startup. Keep Server running? Show status? Show BoardConfiguration page?
  char s[5];
  int loc = sizeof(currentVersion);
  Serial.begin(115200);
  Serial.println("SizeOf: "+String(loc)+"+" + String(sizeof(data)+"+"+String(sizeof(sensorConfig))));
  EEPROM.begin(4096);
  EEPROM.get(0, s);
  if (digitalRead(15) == HIGH) {
    Serial.println("Reset settings");
    wipe();
    delay(500);
  } else if (String(s) != String(currentVersion)) {
    if (String(s) == String(v01String)) {
      updateConfig01To02();
      updateConfig02To03();
    } else if (String(s) == String(v02String)) {
      updateConfig02To03();
    } else {
      wipe();
    }
  } else {
    EEPROM.get(loc, data);
    loc += sizeof(data);
    Serial.println("Loaded");
    if (data.status > 1) {
    }
    if (data.sensorCount > 0) {
      for (int i = 0; i < data.sensorCount; i++) {
        EEPROM.get(loc, sensorConfig[i]);
        loc += sizeof(SensorConfigurationStruct);
      }
      debugPrintSensorConfig();
    }
    debugPrintConfig(true, true, data.status > 1);
  }
  sensorsInitialized = false;
}

void BoardConfiguration::wipe() {
  data.wifiConfig.ssid[0] = 0;
  data.wifiConfig.password[0] = 0;
  data.sensorCount = 0;
  data.status = 0;
  data.mqttConfig.server[0] = 0;
  data.mqttConfig.user[0] = 0;
  data.mqttConfig.password[0] = 0;
  data.mqttConfig.baseTopic[0] = 0;
  data.mqttConfig.useSSL = false;
  data.mqttConfig.port = 0;
  data.mqttConfig.readInterval = 5;
  data.enableDeepSleep = false;
  saveSensorConfigurationStruct();
}

void BoardConfiguration::updateConfig02To03() {
  int loc = sizeof(currentVersion);
  EEPROM.get(loc, data);
  if (data.sensorCount > 0 ) {
    for (int i = 0; i < data.sensorCount; i++) {
      SensorConfigurationStruct01 sensorConfig01;
      EEPROM.get(loc, sensorConfig01);
      std::copy(std::begin(sensorConfig01.sensorName), std::end(sensorConfig01.sensorName), std::begin(sensorConfig[i].sensorName));
      String sensorType;
      String configString;
      switch (sensorConfig01.sensorType) {
        case DHT22_COMPATIBLE:
          sensorType = String("DHT22");
          configString = "[{\"name\":\"Pin\",\"value\":"+String(sensorConfig01.pin)+"}]";
          break;
        case BME280:
          sensorType = String("BME280");
          configString = "[{\"name\":\"Log sea level pressure\",\"value\":\"false\"},{\"name\":\"Altitude\",\"value\":\"0\"}]";
          break;
      }
      configString.toCharArray(sensorConfig[i].configString,1024);
      sensorType.toCharArray(sensorConfig[i].sensorType,256);
      loc += sizeof(SensorConfigurationStruct01);
    }
  }
  saveSensorConfigurationStruct();
}

void BoardConfiguration::updateConfig01To02() {
  ConfigurationStruct01 data01;
  int loc = sizeof(currentVersion);
  EEPROM.get(loc, data01);
  if (data01.sensorCount > 0 ) {
    for (int i = 0; i < data01.sensorCount; i++) {
      EEPROM.get(loc, sensorConfig[i]);
      loc += sizeof(SensorConfigurationStruct);
    }
  }
  data.mqttConfig = data01.mqttConfig;
  data.sensorCount = data01.sensorCount;
  data.status = data01.status;
  data.enableDeepSleep = false;
  strcpy( data.wifiConfig.ssid, data01.wifiConfig.ssid);
  strcpy( data.wifiConfig.password, data01.wifiConfig.password );
  data.wifiConfig.enableAP = true;
  saveSensorConfigurationStruct();
}

void BoardConfiguration::debugPrintSensorConfig() {
  for (int i = 0; i < data.sensorCount; i++) {
    Serial.println("sensorConfig["+String(i)+"].pin : " + String(sensorConfig[i].configString));
    Serial.println("sensorConfig["+String(i)+"].sensorName : " + String(sensorConfig[i].sensorName));
    Serial.println("sensorConfig["+String(i)+"].sensorType : " + String(sensorConfig[i].sensorType));
  }
}

void BoardConfiguration::debugPrintConfig(bool printData, bool printWifi, bool printMQTT) {
  if (printData) {
    Serial.println("data.status : " + String(data.status));
    Serial.println("data.sensorCount : " + String(data.sensorCount));
    Serial.println("data.enableDeepSleep : " + String(data.enableDeepSleep));
  }
  if (printWifi) {
    Serial.println("data.wifiConfig.ssid : " + String(data.wifiConfig.ssid));
    Serial.println("data.wifiConfig.password : " + String(data.wifiConfig.password));
    Serial.println("data.wifiConfig.enableAP : " + String(data.wifiConfig.enableAP));
  }
  if (printMQTT) {
    Serial.println("data.mqttConfig.server : " + String(data.mqttConfig.server));
    Serial.println("data.mqttConfig.port : " + String(data.mqttConfig.port));
    Serial.println("data.mqttConfig.ssl : " + String(data.mqttConfig.useSSL));
    Serial.println("data.mqttConfig.user : " + String(data.mqttConfig.user));
    Serial.println("data.mqttConfig.password : " + String(data.mqttConfig.password));
    Serial.println("data.mqttConfig.baseTopic : " + String(data.mqttConfig.baseTopic));
    Serial.println("data.mqttConfig.readInterval : " + String(data.mqttConfig.readInterval));
  }
}

void BoardConfiguration::save() {
  int loc = sizeof(currentVersion);
  EEPROM.put(0, currentVersion);
  EEPROM.put(loc,data);
  EEPROM.commit();
  Serial.println("Saved");
  debugPrintConfig(true, true, true);
}


void BoardConfiguration::saveSensorConfigurationStruct() {
  save();
  int loc = sizeof(currentVersion);
  loc += sizeof(data);
  for (int i = 0; i < data.sensorCount; i++) {
    EEPROM.put(loc, sensorConfig[i]);
    loc += sizeof(SensorConfigurationStruct);
  }
  EEPROM.commit();
  debugPrintSensorConfig();
  Serial.println("Saved sensorConfig");
}


BoardConfiguration& BoardConfiguration::getInstance() {
  static BoardConfiguration BoardConfiguration;
  return BoardConfiguration;
}

void BoardConfiguration::saveWifiConfiguration(const String& s_ssid, const String& s_password, const bool enableAP) {
  Serial.println("Saving WiFi configuration");
  s_ssid.toCharArray(data.wifiConfig.ssid,32);
  s_password.toCharArray(data.wifiConfig.password,64);
  data.wifiConfig.enableAP = enableAP;
  if (data.status < 1) {
    data.status = 1;
    data.mqttConfig.server[0] = 0;
    data.mqttConfig.user[0] = 0;
    data.mqttConfig.password[0] = 0;
    data.mqttConfig.baseTopic[0] = 0;
    data.mqttConfig.useSSL = false;
    data.mqttConfig.port = 0;
    data.mqttConfig.readInterval = 5;
  }
  save();
}

void BoardConfiguration::saveMQTTConfiguration(const String& s_server, const int port, const bool useSSL, const String& s_user, const String& s_password, const String& s_baseTopic, const int readInterval, const bool enableDeepSleep) {
  Serial.println("Saving MQTT configuration");
  s_server.toCharArray(data.mqttConfig.server,256);
  data.mqttConfig.port=port;
  data.mqttConfig.useSSL=useSSL;
  data.enableDeepSleep = enableDeepSleep;
  s_user.toCharArray(data.mqttConfig.user,64);
  s_password.toCharArray(data.mqttConfig.password,64);
  s_baseTopic.toCharArray(data.mqttConfig.baseTopic,256);
  data.mqttConfig.readInterval=readInterval;
  if (data.status < 2) {
    data.status = 2;
  }
  save();
}

bool BoardConfiguration::connectToWifi() {
  if (data.status < 1) {
    return false;
  }
  WiFi.mode(WIFI_STA);
  Serial.println("Configuration found, connecting to " + String(data.wifiConfig.ssid));
  WiFi.begin(data.wifiConfig.ssid, data.wifiConfig.password);
  unsigned long startMillis = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startMillis < 10000)
  {
    delay(100);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  return true;
}

bool BoardConfiguration::connectToMQTT(PubSubClient &client) {
  if (data.status < 2) {
    return false;
  }
  if (client.loop()) {
      return true;
  } else {
    Serial.println("Board says non connected, state is: "+ String(client.state()));
  }
  if (data.mqttConfig.useSSL) {
    client.setClient(secureNet);
  } else {
    client.setClient(net);
  }
  client.setServer(data.mqttConfig.server, data.mqttConfig.port);
  Serial.println("Connect MQTT");
  connectedToMQTT = client.connect(String(ESP.getChipId()).c_str(), data.mqttConfig.user, data.mqttConfig.password);
  Serial.println("Connection: " + String(connectedToMQTT));
  delay(100);
  client.loop();
  return connectedToMQTT;
}

ConfigurationStruct BoardConfiguration::getConfig() {
  return data;
}

Sensor* getSensor(const SensorConfigurationStruct &sConfig) {
  return SensorManager::createSensor(sConfig);
}

void BoardConfiguration::initSensors(int index=-1) {
  if (index>-1 && !sensorsInitialized) {
    //If sensors aren't initilized yet, init all
    index = -1;
  }
  if (index > -1) {
    Serial.println("Init for index: " + String(index));
    delete sensors[index];
    SensorConfigurationStruct sConfig = sensorConfig[index];
    sensors[index] = getSensor(sConfig);
  } else {
    if (sensorsInitialized) {
      Serial.println("Delete old array of "+ String(createdSensorCount) + " sensors");
      for (int i = 0; i<createdSensorCount; i++) {
        delete sensors[i];
      }
      if (createdSensorCount > 0) {
        delete[] sensors;
      }
    }
    Serial.println("Creating new sensor array for "+ String(data.sensorCount) + " sensors");
    Serial.flush();
    delay(500);
    sensors = new Sensor*[data.sensorCount];
    for (int i = 0; i<data.sensorCount; i++) {
      SensorConfigurationStruct sConfig = sensorConfig[i];
      Serial.println("create sensor: " + i);
      sensors[i] = getSensor(sConfig);
      createdSensorCount++;
    }
    Serial.println("Return");
  }
  sensorsInitialized = data.sensorCount > 0;
}

Sensor** BoardConfiguration::getSensors() {
  if (!sensorsInitialized) {
    Serial.println("!sensorsInitialized");
    initSensors();
  } else {
    Serial.println("sensorsInitialized");
  }
  return sensors;
}
int BoardConfiguration::getSensorCount() {
  return data.sensorCount;
}

int BoardConfiguration::saveSensorConfigurationStruct( int sensorId, const String& sensorType, const String& configString, const String& sensorName) {
  if (sensorId < 0 || sensorId >= getSensorCount()) {
    if (data.sensorCount >= 16) {
      Serial.println("Too many sensors, exiting");
      return -1;
    }
    Serial.println("Existing sensors: " + String(data.sensorCount));
    //New sensor
    configString.toCharArray(sensorConfig[data.sensorCount].configString,1024);
    sensorType.toCharArray(sensorConfig[data.sensorCount].sensorType,256);
    sensorName.toCharArray(sensorConfig[data.sensorCount].sensorName,256);

    Serial.println("Set new data");
    sensorId = data.sensorCount;
    data.sensorCount++;
    Serial.println("now " + String(data.sensorCount) + " sensors");
  } else {
    //Existing sensor
    configString.toCharArray(sensorConfig[sensorId].configString,1024);
    sensorType.toCharArray(sensorConfig[sensorId].sensorType,256);
    sensorName.toCharArray(sensorConfig[sensorId].sensorName,256);
  }
  saveSensorConfigurationStruct();
  if (sensorsInitialized) {
    initSensors(sensorId);
  }
  return sensorId;
}

bool BoardConfiguration::deleteSensorConfigurationStruct(const int sensorId) {
  if (sensorId < 0 || sensorId >= getSensorCount()) {
    return false;
  } else {
    //Sensor exists
    data.sensorCount = data.sensorCount - 1;
    // Shift left existing sensors
    for (int i = sensorId; i < data.sensorCount; i++) {
      std::copy(std::begin(sensorConfig[i+1].sensorType), std::end(sensorConfig[i+1].sensorType), std::begin(sensorConfig[i].sensorType));
      std::copy(std::begin(sensorConfig[i+1].configString), std::end(sensorConfig[i+1].configString), std::begin(sensorConfig[i].configString));
      std::copy(std::begin(sensorConfig[i+1].sensorName), std::end(sensorConfig[i+1].sensorName), std::begin(sensorConfig[i].sensorName));
    }
    sensorConfig[data.sensorCount].sensorType[0] = 0;
    sensorConfig[data.sensorCount].sensorName[0] = 0;
    sensorConfig[data.sensorCount].configString[0] = 0;
    saveSensorConfigurationStruct();
    return true;
  }
}

SensorConfigurationStruct BoardConfiguration::getSensorConfig(const int sensorId) {
  if (sensorId < 0 || sensorId >= getSensorCount()) {
    Serial.println("Index out of range (was: "+String(sensorId)+", max: "+String(getSensorCount())+")");
    Serial.flush();
    panic();
  } else {
    return sensorConfig[sensorId];
  }
}
