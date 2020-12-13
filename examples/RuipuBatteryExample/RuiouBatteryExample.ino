/*
  Software serial multple serial test

  Receives from the hardware serial, sends to software serial.
  Receives from software serial, sends to hardware serial.

  The circuit:
   RX is digital pin 10 (connect to TX of other device)
   TX is digital pin 11 (connect to RX of other device)

  Note:
  Not all pins on the Mega and Mega 2560 support change interrupts,
  so only the following can be used for RX:
  10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

  Not all pins on the Leonardo support change interrupts,
  so only the following can be used for RX:
  8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

  created back in the mists of time
  modified 25 May 2012
  by Tom Igoe
  based on Mikal Hart's example

  This example code is in the public domain.

*/

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
