#include "CordSwitch.h"
#include "Log.h"

CCordSwitch::CCordSwitch(uint8_t pin)
    : mDio(pin)
{
  // Enable pullup because pin is switched to GND
  mDio.SetPullMode(EPullMode::Up);
}

CCordSwitch::~CCordSwitch()
{
}

void CCordSwitch::Sample()
{
  bool value = !mDio.Get();

  if (value == mCurrentValue)
  {
    mStableCount = 0;
  }
  else
  {
    // Filter glitches on input
    if (mStableCount++ > 15)
    {
      mCurrentValue = value;
      mTimer.Reset();
    }
  }
}

ECordState CCordSwitch::GetNextState()
{
  if (mState == ECordState::Idle)
  {
    mPullCount = 0;
    if (mCurrentValue)
      return ECordState::Pulled;
  }

  if (mState == ECordState::Pulled)
  {
    if (!mCurrentValue)
    {
      mPullCount++;
      return ECordState::Released;
    }
    if (mTimer.TimeOut(500))
      return ECordState::Held;
  }

  if (mState == ECordState::Released)
  {
    if (mCurrentValue)
      return ECordState::Pulled;
    if (mTimer.TimeOut(500))
    {
      mFinalPullCount = mPullCount;
      return ECordState::Idle;
    }
  }

  if (mState == ECordState::Held)
  {
    if (!mCurrentValue)
      return ECordState::Idle;
  }

  return mState;
}

void CCordSwitch::Handler()
{
  Sample();
  mState = GetNextState();
}

uint8_t CCordSwitch::GetNumberOfPulls()
{
  auto result = mFinalPullCount;
  mFinalPullCount = 0;
  return result;
}

bool CCordSwitch::IsHeld()
{
  if (mState == ECordState::Held)
    return true;
  return false;
}
