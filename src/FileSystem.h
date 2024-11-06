#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <Arduino.h>
#include "LittleFS.h"

void initFileSystem();
String readFile(const char* path);
void writeFile(const char* path, const char* message);
void deleteFile(const char* path);
bool fileExists(const char* path);

#endif
