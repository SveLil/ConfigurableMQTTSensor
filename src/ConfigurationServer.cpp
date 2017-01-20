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
  size_t sent = webServer.streamFile(file, "text/html");
}

void handleSaveMQTT() {
  int port = webServer.arg("port").toInt();
  bool useSSL = webServer.arg("useSSL") == "true";
  String password = webServer.arg("password");
  if (password == MASK_PASSWORD) {
    ConfigurationStruct config = boardConfig.getConfig();
    password = config.mqttConfig.password;
  }

  boardConfig.saveMQTTConfiguration(  webServer.arg("server"), port, useSSL, webServer.arg("user"), password, webServer.arg("board_name"));
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
  } else {
    webServer.send(500, "application/json","{\"success\": true, \"error\": \"Unknown sensorType: '"+sensorTypeString+"'\"}");
  }

  boardConfig.saveSensorConfiguration( sensorId, sensorType, pin);
  webServer.send(200, "Content-type: application/json", "{\"success\": true, \"sensorId\": "+String(sensorId)+"}");
}

void handleSaveWiFi() {
  String password = webServer.arg("wifi_password");
  if (password == MASK_PASSWORD) {
    ConfigurationStruct config = boardConfig.getConfig();
    password = config.wifiConfig.password;
  }
  boardConfig.saveWifiConfiguration( webServer.arg("ssid"), password);
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

void handleLoad() {
  String json = "{";
  boolean first = true;
  ConfigurationStruct config = boardConfig.getConfig();
  if (config.status > 0) {
    json += "\"ssid\": \"" + String(config.wifiConfig.ssid) + "\", \"wpassword\": \""+MASK_PASSWORD+"\"";
    if (config.status > 1) {
      String ssl = config.mqttConfig.useSSL ? "true" : "false";
      json += ", \"server\": \"" + String(config.mqttConfig.server) + "\", \"port\": \""+String(config.mqttConfig.port)+"\", \"ssl\": \"" + ssl + "\"";
      json += ", \"user\": \"" + String(config.mqttConfig.user) + "\", \"mpassword\": \""+MASK_PASSWORD+"\", \"board_name\": \""+String(config.mqttConfig.boardName) +"\"";
    }
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
