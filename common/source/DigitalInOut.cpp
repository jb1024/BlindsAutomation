#include "DigitalInOut.h"

CDigitalInOut::CDigitalInOut(uint32_t pin)
{
  mPort = g_APinDescription[pin].ulPort;
  mPin = g_APinDescription[pin].ulPin;
  mPinMask = (1ul << mPin);

  // Make input, enable pull, pull to gnd
  PORT->Group[mPort].PINCFG[mPin].reg = (uint8_t)(PORT_PINCFG_INEN | PORT_PINCFG_PULLEN);
  PORT->Group[mPort].DIRCLR.reg = mPinMask;
  PORT->Group[mPort].OUTCLR.reg = mPinMask;
  mIsOutput = false;
}

CDigitalInOut::~CDigitalInOut()
{
}

void CDigitalInOut::Set(bool value)
{
  if (!mIsOutput)
  {
    PORT->Group[mPort].DIRSET.reg = mPinMask;
    mIsOutput = true;
  }

  if (value)
    PORT->Group[mPort].OUTSET.reg = mPinMask;
  else
    PORT->Group[mPort].OUTCLR.reg = mPinMask;
}

bool CDigitalInOut::Get()
{
  if (mIsOutput)
  {
    PORT->Group[mPort].DIRCLR.reg = mPinMask;
    mIsOutput = false;
  }

  if ((PORT->Group[mPort].IN.reg & mPinMask) != 0)
    return true;
  else
    return false;
}

void CDigitalInOut::operator=(bool value)
{
  Set(value);
}
