#include <Arduino.h>
#include <ArduinoJson.h>
#include "config.h"
#include "FileSystem.h"

const char* gatesStatusPath = "/gates_status.txt";

String gatesStatus;

void checkGateStatus() {
    if (digitalRead(openSwitchPin) == HIGH) {
        gatesStatus = "1";
    } else if (digitalRead(closedSwitchPin) == HIGH) {
        gatesStatus = "0";
    } else if (digitalRead(openSwitchPin) == LOW && digitalRead(closedSwitchPin) == LOW) {
        gatesStatus = "2";
    } else {
        gatesStatus = "error";
        Serial.println("Error reading switch pins");
    }

    if (gatesStatus != "error") {
        writeFile(gatesStatusPath, gatesStatus.c_str());
    }

    Serial.println("Gate status: " + gatesStatus);
}

void waitForGateToOpen(unsigned long timeoutMillis) {
    unsigned long startTime = millis();
    bool gateOpened = false;

    while ((millis() - startTime) < timeoutMillis) {
        checkGateStatus();
        if (gatesStatus == "1") {
            gateOpened = true;
            break;
        }
        delay(1000);
        yield();
    }

    if (gateOpened) {
        Serial.println("Gate successfully opened.");
    } else {
        Serial.println("Gate did not open within the expected time.");
    }
}

void waitForGateToClose(unsigned long timeoutMillis) {
    unsigned long startTime = millis();
    bool gateClosed = false;

    while ((millis() - startTime) < timeoutMillis) {
        checkGateStatus();
        if (gatesStatus == "0") {
            gateClosed = true;
            break;
        }
        delay(1000);
        yield();
    }

    if (gateClosed) {
        Serial.println("Gate successfully closed.");
    } else {
        Serial.println("Gate did not close within the expected time.");
    }
}

void checkAndRecoverGateState() {
    esp_reset_reason_t resetReason = esp_reset_reason();
    if (resetReason == ESP_RST_POWERON || resetReason == ESP_RST_BROWNOUT) {
        String previousStatus = readFile(gatesStatusPath);
        Serial.println("Previous gate status: " + previousStatus);
        if (previousStatus == "2") {
            Serial.println("Detected power failure. Reopening the gate to ensure correct state.");

            digitalWrite(relayPin, HIGH);
            delay(1000);
            digitalWrite(relayPin, LOW);

            waitForGateToOpen(90000);

            Serial.println("Closing the gate to ensure correct state.");
            digitalWrite(relayPin, HIGH);
            delay(50);
            digitalWrite(relayPin, LOW);

            waitForGateToClose(90000);

            String output = (gatesStatus == "0") ? "0" : "2";
            writeFile(gatesStatusPath, output.c_str());
        }
    }
}

void checkAndRecoverTask(void *parameter) {
    checkAndRecoverGateState();
    vTaskDelete(NULL);
}

void initiateCheckAndRecover() {
    xTaskCreatePinnedToCore(
        checkAndRecoverTask,
        "CheckAndRecoverTask",
        4096,
        NULL,
        1,
        NULL,
        1
    );

    Serial.println("Asynchronous check and recover initiated.");
}

void gateControlTask(void *parameter) {
    checkAndRecoverGateState();

    digitalWrite(relayPin, HIGH);
    delay(300);
    digitalWrite(relayPin, LOW);
    checkGateStatus();
    delay(500);

    if (gatesStatus == "1") {
        waitForGateToClose(90000);
    } else if (gatesStatus == "0") {
        waitForGateToOpen(90000);
    } else {
        Serial.println("Unknown gate state after command.");
    }

    vTaskDelete(NULL);
}

void initiateGate() {
    xTaskCreatePinnedToCore(
        gateControlTask, 
        "GateControlTask",
        4096,
        NULL,
        1,
        NULL,
        1
    );

    Serial.println("Gate control initiated asynchronously.");
}
