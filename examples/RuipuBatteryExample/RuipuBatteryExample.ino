#include <SoftwareSerial.h>
#include "RuipuBattery.h"

SoftwareSerial mySerial(10, 11); // RX (green wire), TX (blue wire)
RuipuBattery myBattery(&mySerial);

unsigned long timer = 0;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  // Non-blocking timer
  if (millis() - timer > 1000) {
    timer = millis();

    // Send the unlock command
    myBattery.unlock();
    //delay(10);
    myBattery.readStatus();

    // Serial plotter friendly outputs
    Serial.print("Voltage:"); Serial.print(myBattery.packVoltage()); Serial.print(", ");
    Serial.print("Current:"); Serial.print(myBattery.packCurrent()); Serial.print(", ");
    Serial.print("Lowest:"); Serial.print(myBattery.lowestVoltage()); Serial.print(", ");
    Serial.print("Highest:"); Serial.print(myBattery.highestVoltage());

    Serial.println();
  }
}
