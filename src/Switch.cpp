#include "Switch.h"
#include "Log.h"

CSwitch::CSwitch(uint8_t pin)
    : mDio(pin)
{
  // Enable pullup because pin is switched to GND
  mDio.SetPullMode(EPullMode::Up);
}

CSwitch::~CSwitch()
{
}

bool CSwitch::IsOpen()
{
  bool newState = mDio.Get();

  if (newState == mCurrentState)
  {
    mCount = 0;
  }
  else
  {
    // Filter glitches on input
    if (mCount++ > 15)
    {
      mCurrentState = newState;
      Log::Info("Switch state changed to: {}", mCurrentState);
    }
  }

  return mCurrentState;
}

bool CSwitch::IsClosed()
{
  return !IsOpen();
}
