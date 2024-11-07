#ifndef GATESHANDLER_H
#define GATESHANDLER_H

#include <Arduino.h>
#include <ArduinoJson.h>

extern String gatesStatus;

void initiateGate();
void initiateCheckAndRecover();

#endif
