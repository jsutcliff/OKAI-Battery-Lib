/*
  RuipuBattery.cpp - Library for interfacing with Ruipu (OKAI) Scooter Batteries
  Created by Justin Sutcliff, December 13, 2020.
  Released into the public domain.
*/

#include "Arduino.h"
#include "RuipuBattery.h"

RuipuBattery::RuipuBattery(Stream *stream) {
  _stream = stream;
}

byte RuipuBattery::getCRC(const byte *data, byte len) {
  byte crc = 0x00;
  while (len--) {
    byte extract = *data++;
    for (byte tempI = 8; tempI; tempI--) {
      byte sum = (crc ^ extract) & 0x01;
      crc >>= 1;
      if (sum) {
        crc ^= 0x8C;
      }
      extract >>= 1;
    }
  }
  return crc;
}

void RuipuBattery::unlock() {
  byte buf[5] = {0x3A, 0x13, 0x01, 0x16, 0x79};
  _stream->write(buf, sizeof(buf));
}

void RuipuBattery::readStatus() {
  if (_stream->available() >= 36) {
    byte buf[35];
    byte crc;

    _stream->readBytes(buf, sizeof(buf));
    crc = _stream->read();

    byte test = getCRC(buf, sizeof(buf));

    if (crc != test) {
      _stream->flush();
      return;
    }

    _packVoltage = buf[22] << 8 | buf[21];
    _lowCell = buf[32] << 8 | buf[31];
    _highCell = buf[30] << 8 | buf[29];
    _packCurrent = buf[26] << 8 | buf[25];
  }
}

uint16_t RuipuBattery::rawPackVoltage() {
  return _packVoltage;
}

uint16_t RuipuBattery::rawLowestVoltage() {
  return _lowCell;
}

uint16_t RuipuBattery::rawHighestVoltage() {
  return _highCell;
}

int16_t RuipuBattery::rawPackCurrent() {
  return _packCurrent;
}

float RuipuBattery::packVoltage() {
  return _packVoltage / 1000.0;
}

float RuipuBattery::packCurrent() {
  return _packCurrent / 1000.0;
}


float RuipuBattery::lowestVoltage() {
  return _lowCell / 1000.0;
}


float RuipuBattery::highestVoltage() {
  return _highCell / 1000.0;
}
