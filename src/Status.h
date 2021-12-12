#pragma once

#include "Led.h"
#include "Pin.h"

#include <cstdint>

class CStatus
{
public:
  struct SLedPins
  {
    SPin System;
    SPin Red;
    SPin Green;
  };

  CStatus(const SLedPins& leds);
  ~CStatus();

  void Handler();

  // Status flags:
  void SetBooting(bool enable);
  void SetMoving(bool enable);
  void SetAccessPoint(bool enable);

protected:
  CLed mSystemLed;
  CLed mGreenLed;
  CLed mRedLed;
  bool mBooting = true;
  bool mMoving = false;
  bool mAccessPoint = false;
};