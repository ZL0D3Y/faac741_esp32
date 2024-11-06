#ifndef GATESHANDLER_H
#define GATESHANDLER_H

#include <Arduino.h>
#include <ArduinoJson.h>

extern const char* devicesPath;
extern StaticJsonDocument<4096> devices; 

void initiateGate();
void saveDevices();
JsonArray getDevices();

#endif
