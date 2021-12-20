#pragma once

// Types
#include "Pin.h"

// Common
#include "Timer.h"

// Target specific
#include "Pwm.h"

class CAxis
{
public:
  CAxis(const SPin& pin);
  ~CAxis();

  void Initialize();

  // Should be called periodically
  // monitors the setpoint and adjusts the position regarding the speed
  void Handler();

  void SetSpeed(double speed); // Degrees per second
  double GetSpeed();

  void MoveAbsolute(double position); // 0 .. 100%
  void MoveRelative(double movement); // +/- 0 .. 100%
  void Stop(void);

  double GetPosition();

protected:
  CPwm mPwm;
  CTimer mTimer;
  double mPwmMin = 350;
  double mPwmMax = 2500;
  double mSpeed = 10;
  double mCurrentPosition = 50;
  double mTargetPosition = 50;
  bool mEnabled = false;

  void SetPosition(double position);
  void SetPwmForPosition(double position);
  void Enable();
  void Disable();
};