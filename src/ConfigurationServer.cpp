#include <Arduino.h>
#include <FS.h>
#include <ConfigurationServer.h>
#include <SensorConfiguration.h>

ESP8266WebServer webServer = ESP8266WebServer(80);
const int saveTypeWiFi = 0;
const int saveTypeMQTT = 1;

void handleNotFound() {
  Serial.println("Not found:" + webServer.uri());
  webServer.send(404, "text/plain");
}

void handleWeb() {
  Serial.println("Handle Web");
  File file = SPIFFS.open("/init.html", "r");
  size_t sent = webServer.streamFile(file, "text/html");
}

void handleSave(int saveType) {
  String saveTypeString = "unknown";
  switch (saveType) {
    case saveTypeWiFi: saveTypeString = "WiFi"; break;
    case saveTypeMQTT: saveTypeString = "MQTT"; break;
    default: Serial.println("Unknown save type: " + String(saveType));
  }
  Serial.println("Handle Save: " + webServer.uri() + " - " + saveTypeString);
  String msg = "Argument count: " + String(webServer.args());
  msg += "\n";
  for ( uint8_t i = 0; i < webServer.args(); i++ ) {
    msg += " " + webServer.argName ( i ) + ": " + webServer.urlDecode(webServer.arg ( i )) + "\n";
  }
  Serial.println(msg);
  webServer.send(200, "Content-type: application/json", "{\"success\": true}");
}

void handleSaveMQTT() {
  handleSave(saveTypeMQTT);
}

void handleSaveWiFi() {
  handleSave(saveTypeWiFi);
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
  ConfigurationStruct config = SensorConfiguration::getConfig();
  if (config.status > 0) {
    json += "\"ssid\": \"" + String(config.wifiConfig.ssid) + "\", \"wpassword\": \"SAVED\"";
    if (config.status > 1) {
      String ssl = config.mqttConfig.useSSL ? "true" : "false";
      json += ", \"server\": \"" + String(config.mqttConfig.server) + "\", \"port\": \"SAVED\", \"ssl\": \"" + ssl + "\"";
      json += ", \"user\": \"" + String(config.mqttConfig.user) + "\", \"mpassword\": \""+String(config.mqttConfig.user) +"\", \"sensor_name\": \""+String(config.mqttConfig.sensorName) +"\"";
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
  webServer.begin();
  Serial.println("HTTP server started");
}

void ConfigurationServer::handleClient() {
  webServer.handleClient();
}
