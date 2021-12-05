#pragma once

#include "DigitalInOut.h"

class CSwitch
{
public:
  CSwitch(uint8_t pin);
  ~CSwitch();

  bool IsOpen();
  bool IsClosed();

protected:
  bool mCurrentState = false;
  uint8_t mCount = 0;
  CDigitalInOut mDio;
};