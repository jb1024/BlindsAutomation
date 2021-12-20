#pragma once

namespace Interfaces
{

enum class EPullMode
{
  Off,
  Up,
  Down
};

class IDigitalInOut
{
public:
  virtual void SetPullMode(EPullMode mode) = 0;

  virtual void Set(bool value) = 0;
  virtual bool Get() = 0;

  virtual void operator=(bool) = 0;
};

}