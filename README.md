# mqtt_cheerlights
A cheerlights system that's adjustable via MQTT.

## Overview
This is a wifi-connected LED strip that pulls from cheerlights. You can adjust settings via MQTT.

## Parts
- Adafruit Huzzah https://www.adafruit.com/products/2471
- 1m 60/m WS2812B LED Strips http://www.aliexpress.com/item/1m-4m-5m-WS2812B-Smart-led-pixel-strip-Black-White-PCB-30-60-144-leds-m/2036819167.html
- Level Shifter https://www.adafruit.com/products/1787
- Adafruit 1/4 Perma Proto https://www.adafruit.com/products/1608
- 2200 uF capacitor
- Raspberry Pi 3 - Jessie Lite

## Libraries
- PubSubClient https://github.com/knolleary/pubsubclient

## Other

Follow this tutorial to install Mosquitto on your Raspberry Pi from the Debian repositories, which will let you access a much newer version: http://mosquitto.org/2013/01/mosquitto-debian-repository/

I like the apps MQTT Dashboard and MyMQTT, which are free on Google Play for Android. These can be helpful for troubleshooting.
