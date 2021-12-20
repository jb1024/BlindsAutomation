#include "DigitalInOut.h"

using namespace Interfaces;

CDigitalInOut::CDigitalInOut(const SPin& pin)
{
  mPort = g_APinDescription[pin.Pin].ulPort;
  mPin = g_APinDescription[pin.Pin].ulPin;
  mPinMask = (1ul << mPin);
  mInvert = pin.Mode == EPinMode::Inverted;
}

CDigitalInOut::~CDigitalInOut()
{
}

void CDigitalInOut::SetPullMode(EPullMode mode)
{
  uint8_t mask = (uint8_t)PORT_PINCFG_PULLEN;
  auto& pullreg = PORT->Group[mPort].PINCFG[mPin].reg;

  switch (mode)
  {
  case EPullMode::Off:
    pullreg &= ~mask;
    break;
  case EPullMode::Up:
    pullreg |= mask;
    PORT->Group[mPort].OUTSET.reg |= mPinMask;
    break;
  case EPullMode::Down:
    pullreg |= mask;
    PORT->Group[mPort].OUTCLR.reg = mPinMask;
    break;
  }
}

void CDigitalInOut::Set(bool value)
{
  if (!mIsOutput)
  {
    PORT->Group[mPort].DIRSET.reg = mPinMask;
    mIsOutput = true;
  }

  if (mInvert)
    value = !value;

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

  bool result = false;
  if ((PORT->Group[mPort].IN.reg & mPinMask) != 0)
    result = true;
  else
    result = false;

  if (!mInvert)
    return result;
  else
    return !result;
}

void CDigitalInOut::operator=(bool value)
{
  Set(value);
}
