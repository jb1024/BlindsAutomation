#pragma once

#include "Led.h"
#include "Pin.h"

#include <cstdint>

// RVC should this enum class be in a namespace?
enum class ELedOverride
{
  Release,  // Do not override LEDs anymore
  ForceRed, // Force LED to red
  ForceGreen,
  ForceRedGreen,
  ForceOff // Force LED off
};

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
  void SetLed1Override(ELedOverride override);

protected:
  CLed mSystemLed;
  CLed mGreenLed;
  CLed mRedLed;
  bool mBooting = true;
  bool mMoving = false;
  bool mAccessPoint = false;
  bool mOverrideLed1 = false; // Override for LED1 (mGreenLed + mRedLed)
};