#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>

extern const char *configPath;

extern StaticJsonDocument<1024> config;

extern String authUser;
extern String authPass;
extern String WiFiSSID;
extern String WiFiPass;

// Control pins
const int ledPin = 2;
const int resetWifiPin = 25;

// Gates
const int relayPin = 13;
const int openSwitchPin = 33;
const int closedSwitchPin = 34;

bool loadConfig();
void saveConfig();
void resetConfig();

#endif // CONFIG_H
