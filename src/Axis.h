#pragma once

#include <Servo.h>

class CAxis
{
public:
  CAxis(uint8_t pin);
  ~CAxis();

  void SetSpeed(double speed); // Degrees per second
  double GetSpeed();
  void MoveAbsolute(double position); // 0 .. 100%
  void MoveRelative(double movement); // +/- 0 .. 100%
  double GetPosition();

protected:
  uint8_t mPin;
  Servo mServo;
  double mPosition = 50;
  double mSpeed = 10;

  void SetPosition(double position);
};