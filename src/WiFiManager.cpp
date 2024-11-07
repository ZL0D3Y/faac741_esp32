#include "WiFiManager.h"
#include <nvs_flash.h>
#include "FileSystem.h"

const char *ssidPath = "/ssid.txt";
const char *passPath = "/pass.txt";

IPAddress localIP, localGateway, subnet(255, 255, 0, 0);

String ssid, pass;

bool checkCredentialsExists()
{
    if (fileExists(ssidPath) && fileExists(passPath))
    {
        return true;
    }
    return false;
}

void loadWiFiCredentials()
{
    if (fileExists(ssidPath) && fileExists(passPath))
    {
        ssid = readFile(ssidPath);
        pass = readFile(passPath);
    }
}

bool initWiFi()
{
    if (ssid.isEmpty())
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
    WiFi.begin(ssid.c_str(), pass.c_str());
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