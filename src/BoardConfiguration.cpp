#include <BoardConfiguration.h>
#include <ESP8266WiFi.h>
#include "DHTSensor.h"

const char testString[] = "OK3";

BoardConfiguration::BoardConfiguration() {
  // Check status
  // 1: Not configured yet => Show Wifi BoardConfiguration page
  // 2: Wifi configured (and working?) => Connect to Wifi and show MQTT BoardConfiguration page
  // 3: MQTT confiugured => Normal startup. Keep Server running? Show status? Show BoardConfiguration page?
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
    if (data.status > 1) {
      loc += sizeof(data.mqttConfig);
    }
    debugPrintConfig(true, true, data.status > 1);
  }
}

void BoardConfiguration::debugPrintConfig(bool printData, bool printWifi, bool printMQTT) {
  if (printData) {
    Serial.println("data.status : " + String(data.status));
  }
  if (printWifi) {
    Serial.println("data.wifiConfig.ssid : " + String(data.wifiConfig.ssid));
    Serial.println("data.wifiConfig.password : " + String(data.wifiConfig.password));
  }
  if (printMQTT) {
    Serial.println("data.mqttConfig.server : " + String(data.mqttConfig.server));
    Serial.println("data.mqttConfig.port : " + String(data.mqttConfig.port));
    Serial.println("data.mqttConfig.ssl : " + String(data.mqttConfig.useSSL));
    Serial.println("data.mqttConfig.user : " + String(data.mqttConfig.user));
    Serial.println("data.mqttConfig.password : " + String(data.mqttConfig.password));
    Serial.println("data.mqttConfig.boardName : " + String(data.mqttConfig.boardName));
  }
}

void BoardConfiguration::save() {
  int loc = sizeof(testString);
  EEPROM.put(loc,data);
  EEPROM.commit();
  Serial.println("Saved");
  debugPrintConfig(true, true, true);
}

BoardConfiguration& BoardConfiguration::getInstance() {
  static BoardConfiguration BoardConfiguration;
  return BoardConfiguration;
}

void BoardConfiguration::saveWifiConfiguration(const String& s_ssid, const String& s_password) {
  Serial.println("Saving WiFi configuration");
  s_ssid.toCharArray(data.wifiConfig.ssid,32);
  s_password.toCharArray(data.wifiConfig.password,64);
  if (data.status < 1) {
    data.status = 1;
    data.mqttConfig.server[0] = 0;
    data.mqttConfig.user[0] = 0;
    data.mqttConfig.password[0] = 0;
    data.mqttConfig.boardName[0] = 0;
    data.mqttConfig.useSSL = false;
    data.mqttConfig.port = 0;
  }
  save();
}

void BoardConfiguration::saveMQTTConfiguration(const String& s_server, const int port, const bool useSSL, const String& s_user, const String& s_password, const String& s_boardName) {
  Serial.println("Saving MQTT configuration");
  s_server.toCharArray(data.mqttConfig.server,256);
  data.mqttConfig.port=port;
  data.mqttConfig.useSSL=useSSL;
  s_user.toCharArray(data.mqttConfig.user,64);
  s_password.toCharArray(data.mqttConfig.password,64);
  s_boardName.toCharArray(data.mqttConfig.boardName,256);
  if (data.status < 2) {
    data.status = 2;
  }
  save();
}

bool BoardConfiguration::connectToWifi() {
  if (data.status < 1) {
    return false;
  }
  Serial.println("Configuration found, connecting to " + String(data.wifiConfig.ssid));
  WiFi.begin(data.wifiConfig.ssid, data.wifiConfig.password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  return true;
}

bool BoardConfiguration::connectToMQTT(PubSubClient &client) {
  if (data.status < 2) {
    return false;
  }
  if (data.mqttConfig.useSSL) {
    WiFiClientSecure net;
    client.setClient(net);
  } else {
    WiFiClient net;
    client.setClient(net);
  }
  client.setServer(data.mqttConfig.server, data.mqttConfig.port);
  return client.connect(data.mqttConfig.boardName, data.mqttConfig.user, data.mqttConfig.password);
}

ConfigurationStruct BoardConfiguration::getConfig() {
  return data;
}

Sensor* getSensor(SensorConfiguration sConfig) {
  if (sConfig.sensorType == DHT22) {
    return new DHTSensor(sConfig);
  }
}

void BoardConfiguration::initSensors(int index=-1) {
  if (index>-1 && !sensorsInitialized) {
    //If sensors aren't initilized yet, init all
    index = -1;
  }
  if (index > -1) {
    delete sensors[index];
    SensorConfiguration sConfig = sensorConfig[index];
    sensors[index] = getSensor(sConfig);
  } else {
    if (sensorsInitialized) {
        for (int i = 0; i<createdSensorCount; i++) {
          delete sensors[i];
        }
        delete[] sensors;
    }
    sensors = new Sensor*[data.sensorCount];
    for (int i = 0; i<data.sensorCount; i++) {
      SensorConfiguration sConfig = sensorConfig[i];
      sensors[i] = getSensor(sConfig);
    }
  }
  sensorsInitialized = true;
}

Sensor** BoardConfiguration::getSensors() {
  if (!sensorsInitialized) {
    initSensors();
  }
  return sensors;
}
int BoardConfiguration::getSensorCount() {
  return data.sensorCount;
}

void BoardConfiguration::saveSensorConfiguration( int sensorId, const SensorType& sensorType,const int pin) {
  if (sensorId < 0 || sensorId >= getSensorCount()) {
    //New sensor
    SensorConfiguration* newSensorConfig = new SensorConfiguration[data.sensorCount+1];
    memcpy(newSensorConfig, sensorConfig, sizeof(SensorConfiguration)*data.sensorCount);
    newSensorConfig[data.sensorCount].pin = pin;
    newSensorConfig[data.sensorCount].sensorType = sensorType;
    sensorId = data.sensorCount;
    data.sensorCount++;
    delete [] sensorConfig;
    sensorConfig = newSensorConfig;
  } else {
    //Existing sensor
    sensorConfig[sensorId].pin = pin;
    sensorConfig[sensorId].sensorType = sensorType;
    initSensors(sensorId);
  }
}
