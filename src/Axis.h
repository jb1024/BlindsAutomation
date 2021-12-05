#pragma once

#include <Servo.h>

class CAxis
{
public:
  CAxis(uint8_t pin);
  ~CAxis();

  // Should be called periodically
  // monitors the setpoint and adjusts the position regarding the speed
  void Handler();

  void SetSpeed(double speed); // Degrees per second
  double GetSpeed();

  void MoveAbsolute(double position); // 0 .. 100%
  void MoveRelative(double movement); // +/- 0 .. 100%

  double GetPosition();

protected:
  uint8_t mPin;
  Servo mServo;
  double mSpeed = 10;
  double mCurrentPosition = 0;
  double mTargetPosition = 0;
  bool mEnabled = false;
  double mTimer = 0;

  void SetPosition(double position);
  void Enable();
  void Disable();
};