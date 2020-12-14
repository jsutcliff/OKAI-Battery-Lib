# OKAI Battery Interface Library
This library is intended to make interfacing with the batteries found in the OKAI ES200 scooters simple and easy. These batteries appear to be manufactured by 'Ruipu Electronic Technology co.' which is apprently a daughter company to OKAI.

# Usage
The main RuipuBattery class expects a pointer to a Stream in its constructor. See below for exmaples:

Using a HardwareSerial port:
~~~
RuipuBattery myBattery(&Serial);
~~~

Using a SoftwareSerial port:
~~~
SoftwareSerial mySerial(10, 11);
RuipuBattery myBattery(&mySerial);
~~~

Whichever serial port is chose must be initialized to 9600 baud rate.

~~~
Serial.begin(9600);
~~~
or
~~~
mySerial.begin(9600);
~~~

The scooter battery will not output a significant amount of power unless the unlock command is issued. Calling the unlock command every 5 seconds seems to be all that is required to keep the battery operating.

~~~
myBattery.unlock();
~~~

Every time the unlock message is sent to the battery, the BMS will reply with a status message containing the current voltage of the battery, the current flowing in/out if the battery, and the highest and lowest cell voltages. Read in these status messages with the following command:

~~~
myBattery.readStatus();
~~~

The status message can now be accessed through the following methods:

Battery voltage as a float
~~~
myBattery.packVoltage();
~~~

Battery current as a float
~~~
myBattery.packCurrent();
~~~

Highest cell voltage as a float
~~~
myBattery.highestVoltage();
~~~

Lowest cell voltage as a float
~~~
myBattery.lowestVoltage();
~~~

These four values are also available in their two-byte raw forms as recieved from the BMS:

~~~
myBattery.rawPackVoltage();
myBattery.rawPackCurrent();
myBattery.rawLowestVoltage();
myBattery.rawHighestVoltage();
~~~

# Full Example

~~~
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
    
    // Read status message
    myBattery.readStatus();

    // Serial plotter friendly outputs
    Serial.print("Voltage:"); Serial.print(myBattery.packVoltage()); Serial.print(", ");
    Serial.print("Current:"); Serial.print(myBattery.packCurrent()); Serial.print(", ");
    Serial.print("Lowest:"); Serial.print(myBattery.lowestVoltage()); Serial.print(", ");
    Serial.print("Highest:"); Serial.print(myBattery.highestVoltage());
  }
}
~~~
