#include <Arduino.h>
#include <Esp.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <FS.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include "BoardConfiguration.h"
#include "ConfigurationServer.h"
#include "MQTTPublisher.h"
#include "Sensor.h"

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
long lastMsg = 0;

void setup() {
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
  }
  server.start();

  mqttRunning = config.connectToMQTT(client);
}

void loop() {
  if (dnsServerStarted) {
    dnsServer.processNextRequest();
  }
  server.handleClient();
  long now = millis();
  if (now - lastMsg > 100) {
    lastMsg = now;
    mqttPublisher.publish();
  }
}
