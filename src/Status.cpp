#include "Status.h"

CStatus::CStatus(const SLedPins& pins)
    : mSystemLed(pins.System)
    , mGreenLed(pins.Green)
    , mRedLed(pins.Red)
{
  SetBooting(true);
  SetMoving(false);
  SetAccessPoint(false);
}

CStatus::~CStatus()
{
}

void CStatus::SetBooting(bool enable)
{
  mBooting = enable;
  if (mBooting)
    mSystemLed.On();
  else
    mSystemLed.Off();
}

void CStatus::SetMoving(bool enable)
{
  mMoving = enable;
  if (mMoving)
    mRedLed.On();
  else
    mRedLed.Off();
}

void CStatus::SetAccessPoint(bool enable)
{
  mAccessPoint = enable;
  if (mAccessPoint)
    mGreenLed.Blink(1000);
  else
    mGreenLed.Off();
}

void CStatus::Handler()
{
  mSystemLed.Handler();
  mGreenLed.Handler();
  mRedLed.Handler();
}