#pragma once

#include <Servo.h>

class CAxis
{
public:
  CAxis(uint8_t pin);
  ~CAxis();

  void MoveAbsolute(double position); // 0 .. 100%
  void MoveRelative(double movement); // +/- 0 .. 100%
  double GetPosition();

protected:
  Servo mServo;
  double mPosition;

  void SetPosition();
};