#include <Arduino.h>
#include <ArduinoJson.h>
#include "config.h"
#include "FileSystem.h"

const char *gatesStatusPath = "/gates_status.txt";

String gatesStatus;

void checkGateStatus()
{
  if (digitalRead(openSwitchPin) == HIGH)
  {
    gatesStatus = "1";
  }
  else if (digitalRead(closedSwitchPin) == HIGH)
  {
    gatesStatus = "0";
  }
  else if (digitalRead(openSwitchPin) == LOW && digitalRead(closedSwitchPin) == LOW)
  {
    gatesStatus = "2";
  }
  else
  {
    gatesStatus = "error";
    Serial.println("Error reading switch pins");
  }

  if (gatesStatus != "error")
  {
    writeFile(gatesStatusPath, gatesStatus.c_str());
  }
}

bool waitForGateToOpen(unsigned long timeoutMillis)
{
  unsigned long startTime = millis();
  while ((millis() - startTime) < timeoutMillis)
  {
    checkGateStatus();
    if (gatesStatus == "1")
    {
      return true;
    }
    delay(1000);
  }
  return false;
}

bool waitForGateToClose(unsigned long timeoutMillis)
{
  unsigned long startTime = millis();
  while ((millis() - startTime) < timeoutMillis)
  {
    checkGateStatus();
    if (gatesStatus == "0")
    {
      return true;
    }
    delay(1000);
  }

  return false;
}
void initiateGate()
{
  esp_reset_reason_t resetReason = esp_reset_reason();
  if (resetReason == ESP_RST_POWERON || resetReason == ESP_RST_BROWNOUT)
  {
    String previousStatus = readFile(gatesStatusPath);

    if (previousStatus == "2")
    {
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
  checkGateStatus();

  digitalWrite(relayPin, HIGH);
  delay(50);
  digitalWrite(relayPin, LOW);

  if (gatesStatus == "1")
  {
    waitForGateToClose(90000);
  }
  else if (gatesStatus == "0")
  {
    waitForGateToOpen(90000);
  }
  else
  {
    Serial.println("Unknown gate state after command.");
  }
}
