#include <Arduino.h>
#include <Esp.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <FS.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>
#include "BoardConfiguration.h"
#include "ConfigurationServer.h"
#include "MQTTPublisher.h"
#include "Sensor.h"
#include "BME280Sensor.h"
#include "DHTSensor.h"
#include "SHT2XSensor.h"

const byte DNS_PORT = 53;
const char *domain = "esp.sensor";



DNSServer dnsServer;
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);
bool dnsServerStarted = false;
bool mqttRunning = false;
ConfigurationServer server;
PubSubClient client;
BoardConfiguration& config = BoardConfiguration::getInstance();
MQTTPublisher mqttPublisher = MQTTPublisher(client);
unsigned long lastMsg = 0;

void setup() {
  //DHTSensor::registerSensor();
  BME280Sensor::registerSensor();
  //SHT2XSensor::registerSensor();

  Serial.begin(115200);
  Serial.println();
  Serial.println(ESP.getResetReason());
  Serial.println(ESP.getResetInfo());
  // rst_info* rInfo = ESP.getResetInfoPtr();

  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system");
  } else {
    Serial.println("File system mounted");
    if (SPIFFS.exists("/init.html")) {
      Serial.println("File init.html found");
    } else {
      Serial.println("File init.html not found!");
    }
  }

  uint32 realSize = ESP.getFlashChipRealSize();
  Serial.printf("Flash real size: %u\n\n", realSize);

  if (!config.connectToWifi()) {
    Serial.print("Configuring access point...");
    WiFi.softAPConfig(apIP, apIP, netMsk);
    String ssid = "ESP Sensor " + String(ESP.getChipId());
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ssid.c_str());
    delay(500);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    dnsServer.setTTL(300);
    bool dnsStarted = dnsServer.start(DNS_PORT, domain, myIP);
    Serial.println("dnsServer started: " + String(dnsStarted));
    dnsServerStarted = true;
  } else {

  }
  server.start();

  mqttRunning = config.connectToMQTT(client);
  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();

  if (dnsServerStarted) {
    dnsServer.processNextRequest();
  }
  server.handleClient();
  unsigned long diff = ( millis() - lastMsg);
  if (diff > 100) {
    mqttPublisher.publish();
    long sleepS = config.getConfig().mqttConfig.readInterval * 60;
    if (config.getConfig().enableDeepSleep) {
      if (lastMsg > 0) {
        diff = (millis() - lastMsg) - (sleepS * 1000000);
      } else {
        //First run
        diff = 0;
      }
      lastMsg = millis();
      long sleepTime = (sleepS * 1000000)-diff;
      Serial.println("going to sleep for: " + String((sleepTime/1000)) + "ms");
      ESP.deepSleep(sleepTime); //Adjust for processing time
      delay(100);
    } else {
      lastMsg = millis();
    }
  }
}
