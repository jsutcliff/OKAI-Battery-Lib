# OKAI Battery Interface Library
This library is intended to make interfacing with the batteries found in the OKAI ES200 scooters simple and easy. These batteries appear to be manufactured by 'Ruipu Electronic Technology co.' which is apparently a daughter company to OKAI.

# Usage
The main RuipuBattery class expects a pointer to a Stream in its constructor. See below for examples:

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

Every time the unlock message is sent to the battery, the BMS will reply with a status message. In order to read this message, the read() function must be called as often as possible. This function will return true if a status message is successfully read.

~~~
myBattery.read();
~~~

The status message can now be accessed through the following methods:

Battery state of charge (0% - 100%)
~~~
myBattery.soc();
~~~

Battery voltage as a float
~~~
myBattery.voltage();
~~~

Battery current as a float
~~~
myBattery.current();
~~~

Highest cell voltage as a float
~~~
myBattery.high();
~~~

Lowest cell voltage as a float
~~~
myBattery.low();
~~~

Lowest temperature in celsius
~~~
myBattery.minTemp();
~~~

Highest temperature in celsius
~~~
myBattery.maxTemp();
~~~

These four values are also available in their two-byte raw forms as received from the BMS:

~~~
myBattery.rawVoltage();
myBattery.rawCurrent();
myBattery.rawLow();
myBattery.rawHigh();
~~~

# Full Example

~~~
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
~~~
