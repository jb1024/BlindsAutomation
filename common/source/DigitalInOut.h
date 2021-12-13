#pragma once

#include <Arduino.h>

#include <Pin.h>

enum class EPullMode
{
  Off,
  Up,
  Down
};

class CDigitalInOut
{
public:
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
