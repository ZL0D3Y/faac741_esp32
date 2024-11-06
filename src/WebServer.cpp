#include <ESPAsyncWebServer.h> // Видаляємо #include <HTTP_Method.h>
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
    for (int i = 0; i < 10; i++)
    {
      list += "<tr><td> test device " + String(i + 1) + " </td><td> <input type='submit' value='Delete'></td></tr>";
    }
    return list;
  }
  return String();
}

void normalFlow()
{

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/index.html", "text/html", false); });
  server.serveStatic("/", LittleFS, "/");

  server.on("/devices", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/devices.html", "text/html", false, processor); });

  server.on("/devices", HTTP_POST, [](AsyncWebServerRequest *request)
            {
            int params = request->params();
            devices = getDevices();
            for (int i = 0; i < params; i++) {
                AsyncWebParameter* p = request->getParam(i);
                if (p->isPost()) { 
                    if (p->name() == "device") {
                        String deviceName = p->value();
                        devices.add(deviceName);
                    }
                }
            }
              request->redirect("/devices"); });

  server.on("/interact", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    int params = request->params();
    AsyncWebHeader* header = request->getHeader("device");
    if (header) {
        devices = getDevices();
        String device = header->value();
        if (inArray(devices.as<JsonArray>(), device)) {
         initiateGate();
        }
       request->send(200, "text/plain", "Done");
    } else {
        Serial.println("Device header not found");
        request->send(401, "text/plain", "Device not found");
    } });

  server.begin();
}

void setupWiFi()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/wifimanager.html", "text/html"); });

  server.serveStatic("/", LittleFS, "/");

  server.on("/", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    int params = request->params();
    for (int i = 0; i < params; i++) {
      const AsyncWebParameter* p = request->getParam(i);
      if (p->isPost()) {
        if (p->name() == "ssid") {
          ssid = p->value().c_str();
          Serial.print("SSID set to: ");

          Serial.println(ssid);
          writeFile(ssidPath, ssid.c_str());
        }
        if (p->name() == "pass") {
          pass = p->value().c_str();
          writeFile(passPath, pass.c_str());
        }
      }
    }
    request->send(200, "text/plain", "Done. ESP will restart to connect to you router");
    delay(3000);
    ESP.restart(); });
  server.begin();
}
