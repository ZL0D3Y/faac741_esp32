#include "WiFiManager.h"
#include <nvs_flash.h>
#include "FileSystem.h"


IPAddress localIP, localGateway, subnet(255, 255, 0, 0);


bool initWiFi()
{
    if (WiFiSSID.isEmpty())
    {
        Serial.println("Undefined SSID.");
        return false;
    }

    WiFi.mode(WIFI_STA);

    if (!WiFi.config(localIP, localGateway, subnet))
    {
        Serial.println("STA Failed to configure");
        return false;
    }
    WiFi.begin(WiFiSSID.c_str(), WiFiPass.c_str());
    unsigned long previousMillis = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        if (millis() - previousMillis >= 10000)
        {
            Serial.println("Failed to connect.");
            return false;
        }
    }
    return true;
}

void setupAccessPoint()
{
    WiFi.softAP("Gates WIFI", NULL);
}