#pragma once

#include "IPwm.h"
#include "Pin.h"

class CPwm : public Interfaces::IPwm
{
public:
  CPwm(const SPin& pin);
  ~CPwm();

  // IPwm implementatiomn
  void SetFrequency(double Hz) override;
  void SetPulseWidth(double time) override;

protected:
};