#include <SoftwareSerial.h>
#include "RuipuBattery.h"

SoftwareSerial mySerial(10, 11); // RX (green wire), TX (blue wire)
RuipuBattery myBattery(&mySerial);

unsigned long timer = 0;
bool haveReadData = false;

void setup()
{
  Serial.begin(9600);   // Initialize serial for printing results
  mySerial.begin(9600); // Initialize serial for BMS communications
}

void loop()
{
  // Non-blocking timer
  if (millis() - timer > 1000)
  {
    timer = millis();

    myBattery.unlock();   // Send the unlock command
    haveReadData = false; // Reset the read flag
  }

  if (myBattery.read() && !haveReadData)
  {
    haveReadData = true; // Set the read flag

    // Serial plotter friendly outputs
    Serial.print("State of Charge:");
    Serial.print(myBattery.soc());
    Serial.print(", ");
    Serial.print("Voltage:");
    Serial.print(myBattery.voltage());
    Serial.print(", ");
    Serial.print("Current:");
    Serial.print(myBattery.current());
    Serial.print(", ");
    Serial.print("Lowest:");
    Serial.print(myBattery.low());
    Serial.print(", ");
    Serial.print("Highest:");
    Serial.print(myBattery.high());
    Serial.print(", ");
    Serial.print("Max Temp:");
    Serial.print(myBattery.maxTemp());
    Serial.print(", ");
    Serial.print("Min Temp:");
    Serial.print(myBattery.minTemp());
    Serial.print(", ");
    Serial.println();
  }
}