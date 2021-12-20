#pragma once

#include <Arduino.h>

#include <Pin.h>

#include "IDigitalInOut.h"

class CDigitalInOut : public Interfaces::IDigitalInOut
{
public:
  using EPullMode = Interfaces::EPullMode;

  CDigitalInOut(const SPin& pin);
  ~CDigitalInOut();

  void SetPullMode(EPullMode mode);

  void Set(bool value);
  bool Get();

  void operator=(bool);

protected:
  bool mInvert;
  bool mIsOutput = false;
  EPortType mPort = PORTA;
  uint32_t mPin = 0;
  uint32_t mPinMask = 0;
};
