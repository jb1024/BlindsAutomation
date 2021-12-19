#include "Led.h"

#include "Log.h"

CLed::CLed(const SPin& pin)
    : mDio(pin)
{
}

CLed::~CLed()
{
}

void CLed::On()
{
  mState = true;
  mDio = true;
  mBlink = false;
  mTimer.Reset();
}

void CLed::Off()
{
  mState = false;
  mDio = false;
  mBlink = false;
  mTimer.Reset();
}

void CLed::Blink(double delay)
{
  //  Log::Debug("Set blink");
  mState = true;
  mDio = true;
  mDelay = delay;
  mBlink = true;
  mTimer.Reset();
}

void CLed::Handler()
{
  if (mTimer.TimeOut(mDelay))
  {
    mTimer.Reset();
    if (mBlink)
    {
      //  Log::Debug("Toggle");
      mState = !mState;
      mDio = mState;
    }
  }
}
