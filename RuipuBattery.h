/*
  RuipuBattery.h - Library for interfacing with Ruipu (OKAI) Scooter Batteries
  Created by Justin Sutcliff, December 13, 2020.
  Released into the public domain.
*/

#ifndef RuipuBattery_h
#define RuipuBattery_h

#include "Arduino.h"

class RuipuBattery
{
public:
  RuipuBattery(Stream *stream);
  void unlock();
  void reset();
  bool read();

  byte *buf();

  typedef enum ChargerState{
    DISCHARGING,
    BEGIN_CHARGING,
    CHARGING,
    INVALID
  };

  uint16_t rawVoltage();
  uint16_t rawLow();
  uint16_t rawHigh();
  int16_t rawCurrent();

  uint8_t soc();
  uint8_t maxTemp();
  uint8_t minTemp();

  float voltage();
  float current();
  float low();
  float high();

  uint8_t rawStatus();
  bool isChargingBulk();
  bool isCellUndervoltage();
  bool isChargerOK();
  bool isChargerDetected();
  bool isChargeFETEnabled();
  bool isDischargeFETEnabled();

  uint8_t maxCellTemp();
  uint8_t avgCellTemp();
  uint8_t dischargeFETTemp();
  uint8_t microcontrollerTemp();

  uint16_t chargeCycleCount();
  ChargerState chargerState();

private:
  byte crc(const byte *data, byte len);
  Stream *_stream;
  uint8_t _bytesRead;
  byte _buf[36];
};

#endif
