#include <EEPROM.h>
#include <Arduino.h>
#include <ESP8266WebServer.h>

class ConfigurationServer {
public:
    void start();
    void handleClient();    
private:
};
