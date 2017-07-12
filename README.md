# MQTT8266Sensor [![Build Status](https://travis-ci.org/SveLil/MQTT8266Sensor.svg?branch=master)](https://travis-ci.org/SveLil/mqtt8266Sensor) [![Codacy Badge](https://api.codacy.com/project/badge/Grade/0e9e407daa7a449e8e44a02a3651f225)](https://www.codacy.com/app/SveLil/MQTT8266Sensor?utm_source=github.com&utm_medium=referral&utm_content=SveLil/mqtt8266Sensor&utm_campaign=badger)

A ESP8266 based sensor that will send its updates to a MQTT server. The goal of this project was to create one firmware that fits all my sensors at home. It should be possible to do all specific configuration through the web UI.

## Usage

Build and flash to your esp8266 board. When powered on, the board will open a new access point, named "ESP Sensor <ChipId>". After connecting to it, open a web browser and navigate to http://esp.sensor or http://1982.168.4.1

You can then enter the credentials of your network:
![wifi credentials screenshot](https://github.com/SveLil/MQTT8266Sensor/raw/master/doc/wifi.png "wifi credentials Screenshot")

You can either fill in the SSID by hand or scan for existing access points.

After your sensor is connected to a network, you can enter the information for the MQTT server:
![MQTT information screenshot](https://github.com/SveLil/MQTT8266Sensor/raw/master/doc/MQTT.png "MQTT information")

The base topic is used for all sensors connected to the ESP8266. For each sensor, the name will be added, if a sensor has several sub-sensors (like the DTH22). the type (temperature, humidity, etc) will be added, e.g.:

    baseTopic/sensorName/temperature = 26.70
    baseTopic/sensorName/humidity = 43

On the third tab, you can add/edit/delete information about the sensors that are connected to your board:
![empty sensor information screenshot](https://github.com/SveLil/MQTT8266Sensor/raw/master/doc/sensor_empty.png "empty sensor information information")

## TODO
* Cleanup HTML, CSS + JavaScript
* Cleanup serial output
* ~~~Handle 'undefined' configuration options on the front end correctly, instead of displaying 'undefined'~~~
* Implement update function through webserver (see [HTTPUpdateServer](https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266HTTPUpdateServer/src/ESP8266HTTPUpdateServer.cpp))
* Implement OTA update
* Enable sleep mode
  * Trough UI
  * Through physical switch
* ~~~Add physical reset button~~~ (if GPIO 16 is HIGH on startup, config will be reset)
* Add maximum number of sensors in UI
* Show debug output for errors in UI
* Add units to sensor output
* Add sensor specific config (for digital and analog)
