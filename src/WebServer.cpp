#include <ESPAsyncWebServer.h>
#include "FileSystem.h"
#include "WiFiManager.h"
#include "GatesHandler.h"
#include "_helpers.h"
#include "config.h"

AsyncWebServer server(80);

String processor(const String &var)
{
  if (var == "DEVICES_LIST")
  {
    String list = "";

    for (JsonVariant v : devices.as<JsonArray>())
    {
      list += "<tr><td>" + v.as<String>() + "</td><td> <input type='button' onclick=\"submitForm('" + v.as<String>() + "')\" value='Delete'></td></tr>";
    }
    return list;
  }
  return String();
}

void normalFlow()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
              if (!request->authenticate(authUser.c_str(), authPass.c_str())) {
                 return request->requestAuthentication();
              }
              request->send(LittleFS, "/index.html", "text/html", false); });
  server.serveStatic("/", LittleFS, "/");

  server.on("/devices", HTTP_GET, [](AsyncWebServerRequest *request)
            { 
              if (!request->authenticate(authUser.c_str(), authPass.c_str())) {
                 return request->requestAuthentication(); 
              }

              request->send(LittleFS, "/devices.html", "text/html", false, processor); });

  server.on("/devices/delete", HTTP_POST, [](AsyncWebServerRequest *request)
            {
              if (!request->authenticate(authUser.c_str(), authPass.c_str())) {
                 return request->requestAuthentication(); 
              }

              int params = request->params();
              for (int i = 0; i < params; i++) {
                  AsyncWebParameter* p = request->getParam(i);
                  if (p->isPost()) { 
                      if (p->name() == "device") {
                          String deviceToDelete = p->value();
                          JsonArray devicesArray = devices.as<JsonArray>();
                          bool deviceFound = false;

                          for (size_t j = 0; j < devicesArray.size(); j++) {
                              if (devicesArray[j].as<String>() == deviceToDelete) {
                                  devicesArray.remove(j);
                                  deviceFound = true;
                                  break;
                              }
                          }

                          if (deviceFound) {
                              saveDevices(); 
                          }
                      }
                  }
              }
              request->redirect("/devices"); });

  server.on("/devices", HTTP_POST, [](AsyncWebServerRequest *request)
            {
              if (!request->authenticate(authUser.c_str(), authPass.c_str())) {
                  return request->requestAuthentication(); 
              }
              int params = request->params();
              for (int i = 0; i < params; i++) {
                  AsyncWebParameter* p = request->getParam(i);
                  if (p->isPost()) { 
                      if (p->name() == "device") {
                          String deviceName = p->value();
                          if (!inJsonArray(devices.as<JsonArray>(), deviceName)) {
                            devices.add(deviceName);
                            saveDevices();
                          }
                      }
                  }
              }
                request->redirect("/devices"); });

  server.on("/interact", HTTP_POST, [](AsyncWebServerRequest *request)
            {
              int params = request->params();
              AsyncWebHeader* header = request->getHeader("device");
              if (header) {
                  String device = header->value();
                  if (inJsonArray(devices.as<JsonArray>(), device)) {
                      initiateGate();
                  } else {
                      Serial.println("Device not found");
                      request->send(401, "text/plain", "Device not found");
                  }
                request->send(200, "text/plain", "Done");
              } else {
                  Serial.println("Device header not found");
                  request->send(401, "text/plain", "Device not found");
              } });

  server.begin();
}

void setupDevice()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/setup_manager.html", "text/html"); });

  server.serveStatic("/", LittleFS, "/");

  server.on("/", HTTP_POST, [](AsyncWebServerRequest *request)
            {
              int params = request->params();
              for (int i = 0; i < params; i++) {
                const AsyncWebParameter* p = request->getParam(i);
                if (p->isPost()) {
                  if (p->name() == "ssid") {
                    config["ssid"] = p->value().c_str();
                  }
                  if (p->name() == "pass") {
                    config["pass"] = p->value().c_str();
                  }
                  if (p->name() == "esp_user") {
                    config["esp_user"] = p->value().c_str();
                  }
                  if (p->name() == "esp_password") {
                    config["esp_password"] = p->value().c_str();
                  }
                }
              }
              saveConfig();
              request->send(200, "text/plain", "Done. ESP will restart to connect to you router");
              delay(3000);
              ESP.restart(); });

  server.begin();
}
