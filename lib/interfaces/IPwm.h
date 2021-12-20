#pragma once

namespace Interfaces
{

class IPwm
{
public:
  virtual void SetFrequency(double Hz) = 0; // Set frequency in Hz
  virtual void SetPulseWidth(double width) = 0;
};

} // namespace Interfaces