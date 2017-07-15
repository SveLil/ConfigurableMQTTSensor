# ConfigurableMQTTSensor [![Build Status](https://travis-ci.org/SveLil/ConfigurableMQTTSensor.svg?branch=master)](https://travis-ci.org/SveLil/ConfigurableMQTTSensor) [![Codacy Badge](https://api.codacy.com/project/badge/Grade/45854bf6620542cdb1f18a41e51f44c4)](https://www.codacy.com/app/SveLil/ConfigurableMQTTSensor?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=SveLil/ConfigurableMQTTSensor&amp;utm_campaign=Badge_Grade)

A ESP8266 based sensor that will send its updates to a MQTT server. The goal of this project was to create one firmware that fits all my sensors at home. It should be possible to do all specific configuration through the web UI.

## Usage

Build and flash to your esp8266 board. When powered on, the board will open a new access point, named "ESP Sensor <ChipId>". After connecting to it, open a web browser and navigate to http://esp.sensor or http://192.168.4.1

You can then enter the credentials of your network:
![wifi credentials screenshot](https://github.com/SveLil/ConfigurableMQTTSensor/raw/master/doc/wifi.png "wifi credentials Screenshot")

You can either fill in the SSID by hand or scan for existing access points.

After your sensor is connected to a network, you can enter the information for the MQTT server:
![MQTT information screenshot](https://github.com/SveLil/ConfigurableMQTTSensor/raw/master/doc/mqtt.png "MQTT information")

The base topic is used for all sensors connected to the ESP8266. For each sensor, the name will be added, if a sensor has several sub-sensors (like the DTH22). the type (temperature, humidity, etc) will be added, e.g.:

    baseTopic/sensorName/temperature = 26.70
    baseTopic/sensorName/humidity = 43

On the third tab, you can add/edit/delete information about the sensors that are connected to your board:
![empty sensor information screenshot](https://github.com/SveLil/ConfigurableMQTTSensor/raw/master/doc/sensor_empty.png "empty sensor information information")

## TODO
* Cleanup HTML, CSS + JavaScript
* Cleanup serial output
* Implement update function through webserver (see [HTTPUpdateServer](https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266HTTPUpdateServer/src/ESP8266HTTPUpdateServer.cpp))
* Implement OTA update
  * Either via web server or MQTT driven
* Enable sleep mode
  * Through UI
  * Through physical switch
* ~~~Add physical reset button~~~ (if GPIO 16 is HIGH on startup, config will be reset)
* Add maximum number of sensors in UI
* Show debug output for errors in UI
* Add units to sensor output
* Add sensor specific config (for digital and analog)
