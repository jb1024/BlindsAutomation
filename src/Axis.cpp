#include "Axis.h"
#include "Log.h"

#include <Arduino.h>

CAxis::CAxis(uint8_t pin)
    : mPin(pin)
{
}

CAxis ::~CAxis()
{
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

  mTargetPosition = position;
}

double CAxis::GetPosition()
{
  return mCurrentPosition;
}

void CAxis::MoveAbsolute(double position)
{
  Log::Info("Move absolute: {}", position);
  SetPosition(position);
}

void CAxis::MoveRelative(double movement)
{
  Log::Info("Move relative: {}", movement);
  SetPosition(mTargetPosition + movement);
}

void CAxis::Enable()
{
  if (!mMoving)
  {
    Log::Debug("Axis enabled.");
    mServo.attach(mPin);
    mMoving = true;
  }
}

void CAxis::Disable()
{
  if (mMoving)
  {
    delay(500);
    // Give axis minimum time to move
    Log::Debug("Axis disabled.");
    mServo.detach();
    mMoving = false;
  }
}

void CAxis::Handler()
{
  double now = millis();
  double timespan = now - mTimer;

  if (mCurrentPosition == mTargetPosition)
  {
    Disable();
    return;
  }
  Enable();

  double movement = timespan * mSpeed / 1000;
  if (movement > 0)
  {
    mTimer = now;
    if (mTargetPosition > mCurrentPosition)
    {
      mCurrentPosition += movement;
      if (mCurrentPosition > mTargetPosition)
        mCurrentPosition = mTargetPosition;
    }
    else
    {
      mCurrentPosition -= movement;
      if (mCurrentPosition < mTargetPosition)
        mCurrentPosition = mTargetPosition;
    }
  }

  int value = static_cast<int>(mCurrentPosition * 1.8);
  mServo.write(value);
}
