#include <ESPAsyncWebServer.h>
#include "WebServer.h"
#include "FileSystem.h"
#include "WiFiManager.h"
#include "GatesHandler.h"
#include "config.h"

unsigned long startTime = 0;
bool isCounting = false;

void setup()
{
    Serial.begin(115200);

    initFileSystem();

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW);

    pinMode(resetWifiPin, INPUT_PULLDOWN);
    digitalWrite(resetWifiPin, LOW);

    delay(1000);
    if (loadConfig())
    {
        if (initWiFi())
        {
            getDevices();
            normalFlow();
        }
        else
        {
            Serial.println("Failed to initialize WiFi");
        }
    }
    else
    {
        setupAccessPoint();
        setupDevice();
    }
}

void resetDevice()
{
    WiFi.disconnect(true, true);
    resetConfig();
    Serial.println("Device have been fully reseted.");
    delay(3000);
    ESP.restart();
}

void loop()
{
    int signal = digitalRead(resetWifiPin);
    if (signal == HIGH)
    {
        if (!isCounting)
        {
            startTime = millis();
            isCounting = true;
        }

        if (millis() - startTime >= 5000)
        {
            isCounting = false;
            digitalWrite(resetWifiPin, INPUT_PULLDOWN);
            digitalWrite(ledPin, HIGH);
            startTime = 0;
            resetDevice();
        }
    }
    else
    {
        if (isCounting)
        {
            isCounting = false;
            digitalWrite(resetWifiPin, INPUT_PULLDOWN);
        }
        startTime = 0;
    }

    delay(100);
}
