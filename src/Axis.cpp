#include "Axis.h"
#include "Log.h"
#include "System.h"
#include "Timer.h"

#include <Arduino.h>

CAxis::CAxis(const SPin& pin)
    : mPin(pin.Pin)
{
}

CAxis ::~CAxis()
{
}

void CAxis::Initialize()
{
  int value = static_cast<int>(mCurrentPosition * 1.8);
  mServo.write(value);

  Enable();
  delay(500);
  Disable();
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
    Log::Debug("Position clipped {} => 0.0", position);
    position = 0;
  }

  if (position > 100)
  {
    Log::Debug("Position clipped {} => 100.0", position);
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

void CAxis::Stop(void)
{
  mTargetPosition = mCurrentPosition;
  Log::Info("Stop: current position {}", mCurrentPosition);

  SetPosition(mTargetPosition);
}

void CAxis::Enable()
{
  if (!mEnabled)
  {
    Log::Debug("Axis enabled.");
    mServo.attach(mPin);
    mEnabled = true;
    CSystem::Get().GetStatus().SetMoving(true);
  }
}

void CAxis::Disable()
{
  if (mEnabled)
  {
    delay(500);
    // Give axis minimum time to move
    Log::Debug("Axis disabled.");
    mServo.detach();
    mEnabled = false;
    CSystem::Get().GetStatus().SetMoving(false);
  }
}

void CAxis::Handler()
{
  if (!mTimer.TimeOut(10))
    return;

  if (mCurrentPosition == mTargetPosition)
  {
    Disable();
    mTimer.Reset();
    return;
  }

  if (mEnabled == false)
  {
    Enable();
    mTimer.Reset();
    return;
  }

  auto timespan = mTimer.GetTime();
  mTimer.Reset();

  double movement = timespan * mSpeed / 1000;
  // Log::Debug("Movement: {} in timespan {}", movement, timespan);
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

  // remove: Adjust the following constants in the file:  .pio/libdeps/nano_33_iot/Servo/src/Servo.h
  // remove: RVC updated:
  // remove: (the nano_33_iot folder is automatically selected and not visible in VS)
  // Adjust the following constants in the file:  .pio/libdeps/Servo/src/Servo.h
  // #define MIN_PULSE_WIDTH 360
  // #define MAX_PULSE_WIDTH 2500
  int value = static_cast<int>(mCurrentPosition * 1.8); // 100% * 1.8 = 180 degrees
  mServo.write(value);
}
