#include "CordSwitch.h"
#include "Log.h"

CCordSwitch::CCordSwitch(const SPin& pin)
    : mDio(pin)
{
  // Enable pullup because pin is switched to GND
  mDio.SetPullMode(Interfaces::EPullMode::Up);
}

CCordSwitch::~CCordSwitch()
{
}

void CCordSwitch::Sample()
{
  bool value = mDio.Get();

  // Ignore first 10 ms after state change to debounce
  if (!mTimer.TimeOut(10))
    return;

  if (value != mCurrentValue)
  {
    mCurrentValue = value;
    mTimer.Reset();
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
      return ECordState::ReleasedAfterPull;
    }
    if (mTimer.TimeOut(300)) // A held condition detected when held for xx ms
      return ECordState::Held;
  }

  if (mState == ECordState::ReleasedAfterPull)
  {
    if (mCurrentValue)
      return ECordState::Pulled;
    if (mTimer.TimeOut(500)) // Multiple pull counts are counted when released for xx ms
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
