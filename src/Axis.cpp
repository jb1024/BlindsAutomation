#include "Axis.h"
#include "Log.h"

#include <Arduino.h>

CAxis::CAxis(uint8_t pin)
    : mPin(pin)
{
  // mServo.attach(pin);
}

CAxis ::~CAxis()
{
  // mServo.detach();
}

void CAxis::SetSpeed(double speed)
{
  if (speed < 1)
  {
    Log::Error("Value for speed is out of range. Must be > 0");
    return;
  }
  mSpeed = speed;
}

double CAxis::GetSpeed()
{
  return mSpeed;
}

void CAxis::SetPosition(double position)
{
  if (position < 0)
  {
    Log::Warning("Position clipped {} => 0.0", position);
    position = 0;
  }

  if (position > 100)
  {
    Log::Warning("Position clipped {} => 100.0", position);
    position = 100;
  }

  mServo.attach(mPin);

  double start = millis();
  while (mPosition != position)
  {
    double now = millis();
    double timespan = now - start;
    Log::Debug("Timespan: {}", timespan);
    double movement = timespan * mSpeed / 1000;

    if (movement > 0)
    {
      Log::Debug("Movement: {}", movement);
      start = now;

      if (position < mPosition)
      {
        mPosition -= movement;
        if (mPosition < position)
          mPosition = position;
      }
      else
      {
        mPosition += movement;
        if (mPosition > position)
          mPosition = position;
      }
    }

    int value = static_cast<int>(mPosition * 1.8);
    mServo.write(value);
  }

  // Do we need to add additional time here?
  mServo.detach();
}

double CAxis::GetPosition()
{
  return mPosition;
}

void CAxis::MoveAbsolute(double position)
{
  Log::Info("Move absolute: {}", position);
  SetPosition(position);
}

void CAxis::MoveRelative(double movement)
{
  Log::Info("Move relative: {}", movement);
  SetPosition(mPosition + movement);
}
