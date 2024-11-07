#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <config.h>
#include "_helpers.h"

extern String authUser;
extern String authPass;

void normalFlow();
void setupDevice();

#endif
