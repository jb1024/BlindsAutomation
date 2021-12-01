#pragma once

#include <cstdint>

#include "DigitalInOut.h"

enum class ELedMode
{
  Off,
  Red,
  Green
};

class CLeds
{
public:
  CLeds(uint8_t pinRed, uint8_t pinGreen);
  ~CLeds();

  void Handler();

  void SetDelay(uint32_t ms);
  void SetSequence(ELedMode mode1, ELedMode mode2);

protected:
  CDigitalInOut mRed;
  CDigitalInOut mGreen;
  ELedMode mMode1 = ELedMode::Off;
  ELedMode mMode2 = ELedMode::Off;
  bool mToggle = false;
  uint32_t mDelay = 100;
  uint32_t mTime = 0;

  bool TimeOut();
  void SetMode(ELedMode mode);
};