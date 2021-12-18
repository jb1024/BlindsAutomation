#include "Status.h"
#include "Log.h"

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
  Log::Info("SetMoving Called {}", enable);

  if (mOverrideLed1)
    return;

  mMoving = enable;
  if (mMoving)
    mRedLed.On();
  else
    mRedLed.Off();
}

void CStatus::SetAccessPoint(bool enable)
{
  if (mOverrideLed1)
    return;

  mAccessPoint = enable;
  if (mAccessPoint)
    mGreenLed.Blink(1000);
  else
    mGreenLed.Off();
}

void CStatus::SetLed1Override(ELedOverride override)
{
  switch (override)
  {
  case ELedOverride::Release:
    mRedLed.Off();
    mGreenLed.Off();
    mOverrideLed1 = false; // Turn LED off and revert to normal status function
    break;
  case ELedOverride::ForceRed:
    mOverrideLed1 = true;
    mRedLed.On();
    mGreenLed.Off();
    break;
  case ELedOverride::ForceGreen:
    mOverrideLed1 = true;
    mRedLed.Off();
    mGreenLed.On();
    break;
  case ELedOverride::ForceRedGreen:
    mOverrideLed1 = true;
    mRedLed.On();
    mGreenLed.On();
    break;
  case ELedOverride::ForceOff:
    mRedLed.Off();
    mGreenLed.Off();
    mOverrideLed1 = true;
    break;
  }
}

void CStatus::Handler()
{
  mSystemLed.Handler();
  mGreenLed.Handler();
  mRedLed.Handler();
}