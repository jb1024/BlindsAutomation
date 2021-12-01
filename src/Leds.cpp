#include "Leds.h"

#include <Arduino.h>

CLeds::CLeds(uint8_t pinRed, uint8_t pinGreen)
    : mRed(pinRed)
    , mGreen(pinGreen)
{
  SetMode(ELedMode::Off);
}

CLeds::~CLeds()
{
}

bool CLeds::TimeOut()
{
  auto now = millis();
  auto period = now - mTime;
  if (period >= mDelay)
  {
    mTime = now;
    return true;
  }
  return false;
}

void CLeds::SetMode(ELedMode mode)
{
  switch (mode)
  {
  case ELedMode::Red:
    mRed = false;
    mGreen = true;
    break;
  case ELedMode::Green:
    mRed = true;
    mGreen = false;
    break;
  default:
    mRed = true;
    mGreen = true;
  }
}

void CLeds::Handler()
{
  if (TimeOut())
  {
    if (mToggle)
      SetMode(mMode1);
    else
      SetMode(mMode2);
    mToggle = !mToggle;
  }
}

void CLeds::SetDelay(uint32_t ms)
{
  mDelay = ms;
}

void CLeds::SetSequence(ELedMode mode1, ELedMode mode2)
{
  if ((mMode1 == mode1) && (mMode2 == mode2))
    return;

  mMode1 = mode1;
  mMode2 = mode2;

  mToggle = false;
  mTime = millis();
  SetMode(mMode1);
}