#include <Arduino.h>
#include <ArduinoJson.h>
#include "config.h"
#include "FileSystem.h"

const char *devicesPath = "/pass.json";
StaticJsonDocument<4096> devices;

void initiateGate()
{
  digitalWrite(relayPin, HIGH);
  delay(50);
  digitalWrite(relayPin, LOW);
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
