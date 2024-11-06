#include "FileSystem.h"
#include <ArduinoJson.h>


void initFileSystem() {
  if (!LittleFS.begin(true)) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
}

String readFile(const char* path) {
  File file = LittleFS.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return String();
  }

  String fileContent;
  while (file.available()) {
    fileContent = file.readStringUntil('\n');
    break;
  }
  return fileContent;
}

void writeFile(const char* path, const char* message) {
  File file = LittleFS.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (!file.print(message)) {
    Serial.println("- write failed");
  }
}

void deleteFile(const char* path) {
  if (!LittleFS.remove(path)) {
    Serial.println("- failed to delete file");    
  }
}

bool fileExists(const char* path) {
  if (LittleFS.exists(path)) {
    return true;
  } else {
    return false;
  }
}
