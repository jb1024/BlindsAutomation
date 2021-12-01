#pragma once

#include <Arduino.h>

class CDigitalInOut
{
public:
  CDigitalInOut(uint32_t pin);
  ~CDigitalInOut();

  void Set(bool value);
  bool Get();

  void operator=(bool);

protected:
  bool mIsOutput = false;
  EPortType mPort = PORTA;
  uint32_t mPin = 0;
  uint32_t mPinMask = 0;
};
