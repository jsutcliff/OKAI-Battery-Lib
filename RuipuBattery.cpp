/*
  RuipuBattery.cpp - Library for interfacing with Ruipu (OKAI) Scooter Batteries
  Created by Justin Sutcliff, December 13, 2020.
  Released into the public domain.
*/

#include "Arduino.h"
#include "RuipuBattery.h"

RuipuBattery::RuipuBattery(Stream *stream)
{
  _stream = stream;
}

byte RuipuBattery::crc(const byte *data, byte len)
{
  byte crc = 0x00;
  while (len--)
  {
    byte extract = *data++;
    for (byte tempI = 8; tempI; tempI--)
    {
      byte sum = (crc ^ extract) & 0x01;
      crc >>= 1;
      if (sum)
      {
        crc ^= 0x8C;
      }
      extract >>= 1;
    }
  }
  return crc;
}

void RuipuBattery::unlock()
{
  reset();
  byte buf[5] = {0x3A, 0x13, 0x01, 0x16, 0x79};
  _stream->write(buf, sizeof(buf));
}

bool RuipuBattery::read()
{
  // Read all available serial data
  while (_stream->available() > 0)
  {
    byte b = _stream->read();

    if (_bytesRead < 36)
    {
      _buf[_bytesRead] = b;
      _bytesRead++;
    }
  }

  // Check for complete message
  if (_bytesRead > 35)
  {
    byte test = crc(_buf, 35);

    // Check crc
    if (_buf[35] == test)
    {
      return true;
    }

    _bytesRead = 0;
  }

  return false;
}

void RuipuBattery::reset()
{
  while (_stream->available() > 0)
  {
    _stream->read();
  }
  _bytesRead = 0;
}

byte *RuipuBattery::buf()
{
  return _buf;
}

uint8_t RuipuBattery::soc()
{
  return _buf[5];
}

uint16_t RuipuBattery::rawVoltage()
{
  return _buf[22] << 8 | _buf[21];
}

uint16_t RuipuBattery::rawLow()
{
  return _buf[32] << 8 | _buf[31];
}

uint16_t RuipuBattery::rawHigh()
{
  return _buf[30] << 8 | _buf[29];
}

int16_t RuipuBattery::rawCurrent()
{
  return _buf[26] << 8 | _buf[25];
}

float RuipuBattery::voltage()
{
  return rawVoltage() / 1000.0;
}

float RuipuBattery::current()
{
  return rawCurrent() / 1000.0;
}

float RuipuBattery::low()
{
  return rawLow() / 1000.0;
}

float RuipuBattery::high()
{
  return rawHigh() / 1000.0;
}

uint8_t RuipuBattery::maxTemp()
{
  uint8_t maxTemp = 0;

  for (int i = 0; i < 4; i++)
  {
    if (_buf[7 + i] > maxTemp)
    {
      maxTemp = _buf[7 + i];
    }
  }

  return maxTemp;
}

uint8_t RuipuBattery::minTemp()
{
  uint8_t minTemp = 255;

  for (int i = 0; i < 4; i++)
  {
    if (_buf[7 + i] < minTemp)
    {
      minTemp = _buf[7 + i];
    }
  }

  return minTemp;
}