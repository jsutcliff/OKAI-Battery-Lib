/*
  RuipuBattery.h - Library for interfacing with Ruipu (OKAI) Scooter Batteries
  Created by Justin Sutcliff, December 13, 2020.
  Released into the public domain.
*/

#ifndef RuipuBattery_h
#define RuipuBattery_h

#include "Arduino.h"

class RuipuBattery {
  public:
    RuipuBattery(Stream *stream);
    void unlock();
    void readStatus();
    
    uint16_t rawPackVoltage();
    uint16_t rawLowestVoltage();
    uint16_t rawHighestVoltage();
    int16_t rawPackCurrent();

    float packVoltage();
    float packCurrent();
    float lowestVoltage();
    float highestVoltage();
    
  private:
    byte getCRC(const byte *data, byte len);
    Stream *_stream;
    uint16_t _packVoltage, _lowCell, _highCell;
    int16_t _packCurrent;
};

#endif
