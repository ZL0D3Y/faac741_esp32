#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>
#include <IPAddress.h>

extern const char* ssidPath;
extern const char* passPath;

extern String ssid;
extern String pass;


bool checkCredentialsExists();
void loadWiFiCredentials();
bool initWiFi();
void setupAccessPoint();
void resetWiFiSettings();

#endif
