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

    pinMode(openSwitchPin, INPUT_PULLDOWN);
    pinMode(closedSwitchPin, INPUT_PULLDOWN);

    delay(1000);
    if (loadConfig())
    {
        if (initWiFi())
        {
            getDevices();
            initiateCheckAndRecover();
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
    resetDeviceConfig();
    Serial.println("Device have been fully reseted.");
    delay(3000);
    ESP.restart();
}

void loop()
{
    if (digitalRead(resetWifiPin) == HIGH)
    {
        if (!isCounting)
        {
            startTime = millis();
            isCounting = true;
        }

        if (millis() - startTime >= 3000)
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
