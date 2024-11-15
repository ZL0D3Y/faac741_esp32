#include "config.h"
#include "FileSystem.h"
#include <ArduinoJson.h>
#include <Arduino.h>

const char *configPath = "/config.json";
const char *devicesPath = "/devices.json";

String authUser;
String authPass;
String WiFiSSID;
String WiFiPass;

StaticJsonDocument<4096> devices;
StaticJsonDocument<1024> config;

bool loadConfig()
{
    if (fileExists(configPath))
    {
        String configRaw = readFile(configPath);
        DeserializationError error = deserializeJson(config, configRaw);
        if (error)
        {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.f_str());
            deleteFile(configPath);
            return false;
        }

        authUser = config["esp_user"].as<String>();
        authPass = config["esp_password"].as<String>();
        WiFiSSID = config["ssid"].as<String>();
        WiFiPass = config["pass"].as<String>();
        return true;
    }
    return false;
}

void saveConfig()
{
    String output;
    serializeJson(config, output);
    writeFile(configPath, output.c_str());
}

void resetDeviceConfig()
{
    deleteFile(configPath);
    deleteFile(devicesPath);
}


JsonArray getDevices()
{
  if (fileExists(devicesPath))
  {
    String devicesRaw = readFile(devicesPath);
    DeserializationError error = deserializeJson(devices, devicesRaw);
    if (error)
    {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.f_str());
      deleteFile(devicesPath);
      return JsonArray();
    }

    return devices.as<JsonArray>();
  }

  return JsonArray();
}

void saveDevices()
{
  String output;
  serializeJson(devices, output);
  writeFile(devicesPath, output.c_str());
}
