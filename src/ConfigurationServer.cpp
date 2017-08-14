#include <Arduino.h>
#include <FS.h>
#include <ConfigurationServer.h>
#include <BoardConfiguration.h>

ESP8266WebServer webServer = ESP8266WebServer(80);
const int saveTypeWiFi = 0;
const int saveTypeMQTT = 1;
const String MASK_PASSWORD = "SAVED";
BoardConfiguration& boardConfig = BoardConfiguration::getInstance();

void handleNotFound() {
  Serial.println("Not found:" + webServer.uri());
  webServer.send(404, "text/plain");
}

void handleWeb() {
  Serial.println("Handle Web");
  File file = SPIFFS.open("/init.html", "r");
  webServer.streamFile(file, "text/html");
}

void handleSaveMQTT() {
  int port = webServer.arg("port").toInt();
  int interval = webServer.arg("interval").toInt();
  bool useSSL = webServer.arg("useSSL") == "true";
  bool enableDeepSleep = webServer.arg("enableDeepSleep") == "true";
  String password = webServer.arg("password");
  if (password == MASK_PASSWORD) {
    ConfigurationStruct config = boardConfig.getConfig();
    password = config.mqttConfig.password;
  }

  boardConfig.saveMQTTConfiguration(  webServer.arg("server"), port, useSSL, webServer.arg("user"), password, webServer.arg("baseTopic"),interval, enableDeepSleep);
  webServer.send(200, "Content-type: application/json", "{\"success\": true}");
}

void handleDeleteSensor() {
  int sensorId = webServer.arg("sensorId").toInt();
  boardConfig.deleteSensorConfiguration( sensorId );
  webServer.send(200, "Content-type: application/json", "{\"success\": true}");
}

void handleSaveSensor() {
  Serial.println("Handle Save: " + webServer.uri() + " - Sensor");
  String msg = "Argument count: " + String(webServer.args());
  msg += "\n";
  for ( uint8_t i = 0; i < webServer.args(); i++ ) {
    msg += " " + webServer.argName ( i ) + ": " + webServer.urlDecode(webServer.arg ( i )) + "\n";
  }
  Serial.println(msg);

  int sensorId = webServer.arg("sensorId").toInt();
  int pin = webServer.arg("pin").toInt();
  String sensorTypeString = webServer.arg("sensorType");
  SensorType sensorType;
  if (sensorTypeString == "DHT22") {
    sensorType = DHT22_COMPATIBLE;
  } else if (sensorTypeString == "Analog") {
    sensorType = SIMPLE_ANALOG;
  } else if (sensorTypeString == "Digital") {
    sensorType = SIMPLE_DIGITAL;
  } else if (sensorTypeString == "BME280") {
    sensorType = BME280;
  } else {
    webServer.send(500, "application/json","{\"success\": false, \"error\": \"Unknown sensorType: '"+sensorTypeString+"'\"}");
  }
  String sensorName = webServer.arg("sensorName");

  sensorId = boardConfig.saveSensorConfiguration( sensorId, sensorType, pin, sensorName);
  if (sensorId == -1) {
    webServer.send(500, "Content-type: application/json", "{\"success\": false, \"error\": \"Too many sensors\"}");
  } else {
    webServer.send(200, "Content-type: application/json", "{\"success\": true, \"sensorId\": "+String(sensorId)+"}");
  }
}

void handleSaveWiFi() {
  String password = webServer.arg("wifi_password");
  if (password == MASK_PASSWORD) {
    ConfigurationStruct config = boardConfig.getConfig();
    password = config.wifiConfig.password;
  }
  bool enableAP = webServer.arg("enableAP") == "true";

  boardConfig.saveWifiConfiguration( webServer.arg("ssid"), password,  enableAP);
  webServer.send(200, "Content-type: application/json", "{\"success\": true}");
}

void handleScan() {
  int nCount = WiFi.scanNetworks();
  String json = "[";
  boolean first = true;
  for (int i = 0; i < nCount; i++) {
    String ssid = WiFi.SSID(i);
    uint8_t encryptionType = WiFi.encryptionType(i);
    String encryptionString;
    switch (encryptionType) {
      case ENC_TYPE_AUTO: encryptionString = "Auto"; break;
      case ENC_TYPE_CCMP: encryptionString = "WPA2"; break;
      case ENC_TYPE_NONE: encryptionString = "Open"; break;
      case ENC_TYPE_TKIP: encryptionString = "WPA"; break;
      case ENC_TYPE_WEP: encryptionString = "WEP"; break;
      default: encryptionString = "unknown";
    }
    if (first) {
      first = false;
    } else {
      json += ",";
    }
    json += "{ \"ssid\": \"" + ssid + "\", \"type\": \"" + encryptionString + "\" }";
  }
  json +="]";
  webServer.send(200, "Content-type: application/json", json);
  WiFi.scanDelete();
}

String boolToString(bool val) {
  return val ? "true" : "false";
}

void handleLoad() {
  String json = "{";
  boolean first = true;
  ConfigurationStruct config = boardConfig.getConfig();
  if (config.status > 0) {
    json += "\"ssid\": \"" + String(config.wifiConfig.ssid) + "\", \"wpassword\": \""+MASK_PASSWORD+"\"";
    json += ", \"enableAP\": \""+boolToString(config.wifiConfig.enableAP) +"\", \"enableDeepSleep\": \""+boolToString(config.enableDeepSleep) +"\"";
    if (config.status > 1) {
      json += ", \"server\": \"" + String(config.mqttConfig.server) + "\", \"port\": \""+String(config.mqttConfig.port)+"\", \"ssl\": \"" + boolToString(config.mqttConfig.useSSL) + "\"";
      json += ", \"user\": \"" + String(config.mqttConfig.user) + "\", \"mpassword\": \""+MASK_PASSWORD+"\"";
      json += ", \"baseTopic\": \""+String(config.mqttConfig.baseTopic) +"\", \"interval\": \""+String(config.mqttConfig.readInterval) +"\"";
    }
    json += ", \"sensors\": [";
    for (int i = 0; i < config.sensorCount; i++) {
      if (first) {
        first = false;
      } else {
        json += ",";
      }
      SensorConfiguration sensorConfig = boardConfig.getSensorConfig(i);
      String sensorType;
      if (sensorConfig.sensorType == DHT22_COMPATIBLE) {
        sensorType = "DHT22";
      } else if (sensorConfig.sensorType == SIMPLE_ANALOG) {
        sensorType = "Analog";
      } else if (sensorConfig.sensorType == SIMPLE_DIGITAL) {
        sensorType = "Digital";
      } else if (sensorConfig.sensorType == BME280) {
        sensorType = "BME280";
      }
      json += "{";
      json += " \"sensorType\": \""+sensorType+ "\"";
      json += ", \"pin\": \""+ String(sensorConfig.pin)+"\"";
      json += ", \"sensorName\": \""+String(sensorConfig.sensorName)+"\"";
      json += ", \"sensorId\": \""+String(i)+"\"";
      json += "}";
    }
    json += "]";

  }
  json +="}";
  webServer.send(200, "Content-type: application/json", json);
}

void ConfigurationServer::start() {
  webServer.onNotFound(handleNotFound);
  webServer.on("/", HTTP_GET, handleWeb);
  webServer.on("/init.html", HTTP_GET, handleWeb);
  webServer.on("/saveMQTT", HTTP_POST, handleSaveMQTT);
  webServer.on("/saveWiFi", HTTP_POST, handleSaveWiFi);
  webServer.on("/scan", HTTP_GET, handleScan);
  webServer.on("/load", HTTP_GET, handleLoad);
  webServer.on("/deleteSensor", HTTP_POST, handleDeleteSensor);
  webServer.on("/saveSensor", HTTP_POST, handleSaveSensor);
  webServer.begin();
  Serial.println("HTTP server started");
}

void ConfigurationServer::handleClient() {
  webServer.handleClient();
}
