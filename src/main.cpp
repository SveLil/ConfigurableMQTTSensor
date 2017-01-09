#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <SimpleDHT.h>
#include <SensorConfiguration.h>
#include <ConfigurationServer.h>
#include <FS.h>
#include <ESP8266WebServer.h>


const byte DNS_PORT = 53;
const char *domain = "esp.sensor";

DNSServer dnsServer;
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);
bool dnsServerStarted = false;
ConfigurationServer server;

void setup() {
  Serial.begin(115200);
  Serial.println();
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
  if (false) { //!SensorConfiguration::isWifiConfigured()
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
    SensorConfiguration::connectToWifi();
  }
  server.start();
}

void loop() {
  if (dnsServerStarted) {
    dnsServer.processNextRequest();
  }
  server.handleClient();
}
