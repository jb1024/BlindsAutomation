#pragma once

#include "DigitalInOut.h"
#include "Pin.h"
#include "Timer.h"

enum class ECordState
{
  Idle,              // Waiting for switch
  Pulled,            // Cord is pulled
  ReleasedAfterPull, // Release and waiting for another click
  Held,              // Cord is held for a longer time
};

class CCordSwitch
{
public:
  CCordSwitch(const SPin& pin);
  ~CCordSwitch();

  void Handler();

  uint8_t GetNumberOfPulls();
  bool IsHeld();

protected:
  CDigitalInOut mDio;
  CTimer mTimer;
  bool mCurrentValue = false;
  ECordState mState = ECordState::Idle;
  uint32_t mPullCount = 0;
  uint32_t mFinalPullCount = 0;

  void Sample();
  ECordState GetNextState();
};