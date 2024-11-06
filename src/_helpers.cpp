#include <Arduino.h>
#include <ArduinoJson.h>

bool inArray(JsonArray array, const String& value) {
  for (JsonVariant v : array) {
    if (v.as<String>() == value) {
      return true; 
    }
  }
  return false; 
}