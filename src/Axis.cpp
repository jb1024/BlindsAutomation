#include "Axis.h"
#include "Log.h"

CAxis::CAxis(uint8_t pin)
{
  mServo.attach(pin);
}

CAxis ::~CAxis()
{
  mServo.detach();
}

void CAxis::SetPosition()
{
  if (mPosition < 0)
  {
    Log::Warning("Position clipped {} => 0.0", mPosition);
    mPosition = 0;
  }

  if (mPosition > 100)
  {
    Log::Warning("Position clipped {} => 100.0", mPosition);
    mPosition = 100;
  }

  // Translate percentage to PWM value
  // 0% = 0;
  // 100% = 180;
  int value = static_cast<int>(mPosition * 1.8);
  mServo.write(value);
  Log::Info("Current position: {}", mPosition);
}

void CAxis::MoveAbsolute(double position)
{
  Log::Info("Move absolute: {}", position);
  mPosition = position;
  SetPosition();
}

void CAxis::MoveRelative(double movement)
{
  Log::Info("Move relative: {}", movement);
  mPosition += movement;
  SetPosition();
}

double CAxis::GetPosition()
{
  return mPosition;
}
