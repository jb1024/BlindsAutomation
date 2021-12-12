#pragma once

#include <cstdint>

#include "DigitalInOut.h"
#include "Pin.h"
#include "Timer.h"

class CLed
{
public:
  CLed(const SPin& pin);
  ~CLed();

  void Handler();

  void On();
  void Off();
  void Blink(double delay);

protected:
  CDigitalInOut mDio;
  bool mState = false;
  bool mBlink = false;
  double mDelay = 0;
  CTimer mTimer;
};