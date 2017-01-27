#include <BoardConfiguration.h>
#include <ESP8266WiFi.h>
#include "DHTSensor.h"

const char testString[] = "OK4";

BoardConfiguration::BoardConfiguration() {
  // Check status
  // 1: Not configured yet => Show Wifi BoardConfiguration page
  // 2: Wifi configured (and working?) => Connect to Wifi and show MQTT BoardConfiguration page
  // 3: MQTT confiugured => Normal startup. Keep Server running? Show status? Show BoardConfiguration page?
  char s[4];
  int loc = sizeof(s);
  Serial.begin(115200);
  Serial.println("SizeOf: "+String(loc)+"+" + String(sizeof(data)+"+"+String(sizeof(sensorConfig))));
  EEPROM.begin(4096);
  EEPROM.get(0, s);
  bool reset = false;
  if (digitalRead(16) == HIGH) {
    //Reset
    reset = true;
    Serial.println("Reset settings");
  }
  if (reset || String(s) != String(testString)) {
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
    loc += sizeof(data);
    Serial.println("Loaded");
    if (data.status > 1) {
    }
    debugPrintConfig(true, true, data.status > 1);
    if (data.sensorCount > 0) {
      EEPROM.get(loc, sensorConfig);
    }
  }
  sensorsInitialized = false;
}

void BoardConfiguration::debugPrintConfig(bool printData, bool printWifi, bool printMQTT) {
  if (printData) {
    Serial.println("data.status : " + String(data.status));
    Serial.println("data.sensorCount : " + String(data.sensorCount));
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
    Serial.println("data.mqttConfig.readInterval : " + String(data.mqttConfig.readInterval));
  }
}

void BoardConfiguration::save() {
  int loc = sizeof(testString);
  EEPROM.put(loc,data);
  EEPROM.commit();
  Serial.println("Saved");
  debugPrintConfig(true, true, true);
}


void BoardConfiguration::saveSensorConfiguration() {
  save();
  int loc = sizeof(testString);
  loc += sizeof(data);
  EEPROM.put(loc,sensorConfig);
  EEPROM.commit();
  Serial.println("Saved sensorConfig");
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
    data.mqttConfig.readInterval = 5;
  }
  save();
}

void BoardConfiguration::saveMQTTConfiguration(const String& s_server, const int port, const bool useSSL, const String& s_user, const String& s_password, const String& s_boardName, const int readInterval) {
  Serial.println("Saving MQTT configuration");
  s_server.toCharArray(data.mqttConfig.server,256);
  data.mqttConfig.port=port;
  data.mqttConfig.useSSL=useSSL;
  s_user.toCharArray(data.mqttConfig.user,64);
  s_password.toCharArray(data.mqttConfig.password,64);
  s_boardName.toCharArray(data.mqttConfig.boardName,256);
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
    Serial.println("MQTT not configured");
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
  Serial.println("Connect MQTT");
  connectedToMQTT = client.connect(data.mqttConfig.boardName, data.mqttConfig.user, data.mqttConfig.password);
  Serial.println("Connection: " + String(connectedToMQTT));
  return connectedToMQTT;
}

ConfigurationStruct BoardConfiguration::getConfig() {
  return data;
}

Sensor* getSensor(const SensorConfiguration &sConfig) {
  if (sConfig.sensorType == DHT22_COMPATIBLE) {
    Serial.println("Created DHT22 sensor");
    return new DHTSensor(sConfig);
  } else {
    Serial.println("Unknown sensor: " + String(sConfig.sensorType) + " not: " + String(DHT22_COMPATIBLE));
    return NULL;
  }
}

void BoardConfiguration::initSensors(int index=-1) {
  if (index>-1 && !sensorsInitialized) {
    //If sensors aren't initilized yet, init all
    index = -1;
  }
  if (index > -1) {
    Serial.println("Init for index: " + String(index));
    delete sensors[index];
    SensorConfiguration sConfig = sensorConfig[index];
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
      SensorConfiguration sConfig = sensorConfig[i];
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

void BoardConfiguration::saveSensorConfiguration( int sensorId, const SensorType& sensorType,const int pin) {
  if (sensorId < 0 || sensorId >= getSensorCount()) {
    if (data.sensorCount >= 16) {
      Serial.println("Too many sensors, exiting");
      return;
    }
    Serial.println("New sensor for pin" + String(pin) + " of type " + String(sensorType));
    Serial.println("Existing sensors: " + String(data.sensorCount));
    //New sensor
    sensorConfig[data.sensorCount].pin = pin;
    sensorConfig[data.sensorCount].sensorType = sensorType;
    Serial.println("Set new data");
    sensorId = data.sensorCount;
    data.sensorCount++;
    Serial.println("now " + String(data.sensorCount) + " sensors");
  } else {
    //Existing sensor
    sensorConfig[sensorId].pin = pin;
    sensorConfig[sensorId].sensorType = sensorType;
  }
  if (sensorsInitialized) {
    initSensors(sensorId);
  }
  saveSensorConfiguration();
  Serial.println("Return");
}

void BoardConfiguration::deleteSensorConfiguration(const int sensorId) {
  if (sensorId < 0 || sensorId >= getSensorCount()) {
  } else {
    //Sensor exists
    //Mark sensor config as empty
    sensorConfig[sensorId].sensorType = NO_SENSOR;
  }
}
