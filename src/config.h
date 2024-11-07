#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>

// File paths
extern const char *configPath;
extern const char* devicesPath;

extern StaticJsonDocument<1024> config;
extern StaticJsonDocument<4096> devices; 


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
const int closedSwitchPin = 32;


void saveDevices();
JsonArray getDevices();
bool loadConfig();
void saveConfig();
void resetDeviceConfig();

#endif // CONFIG_H
