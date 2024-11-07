#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <ESPAsyncWebServer.h>

extern const int ledPin;

extern const char* authUserPath;
extern const char* authPassPath;

extern const char *authUser;
extern const char *authPass;


void normalFlow();
void setupDevice();

#endif
