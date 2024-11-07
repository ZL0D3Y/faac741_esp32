#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>
#include <IPAddress.h>
#include <config.h>

extern String WiFiSSID;
extern String WiFiPass;

bool initWiFi();
void setupAccessPoint();
void resetDevice();

#endif
