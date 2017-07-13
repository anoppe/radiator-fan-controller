# Radiator Fan Controller
This repository contains an Arduino IDE sketch wich makes it possible to toggle a fan based on the radiator temperature.

The reason I started this project is because I build a radiator-cover so I didn't have to look at that ugly metal thing. Such cover blocks a lot of heat from the radiator which is meant to raise the temperature of the room the radiator is placed in. I've searched on the internet for solutions for this and it seems that one can buy devices that do this for you, but DYI is much more fun!

## Requirements
**Radiator Fan Controller** depends on the Dallas digital temperature sensor (`DS18B20`) to detect wether the fan has to be turned on (temp >= 30 degree C) or off (temp < 30 degree C).
Based on the input of the temperature sensor a digital port is toggled high which will switch a Relay module the fans are connected to. Since the fans run on 12v and the esp module I'm using, I've put a relay module in between.

## Future work

* Add WiFi connectivity
* When fan toggles, send notification to IoT backend to be able to visualise the events

