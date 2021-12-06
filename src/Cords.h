#pragma once

#include <cstdint>
#include <functional>
#include <map>

#include "CordSwitch.h"

enum class ECordsAction
{
  None,
  BothHeld,
  Held1,
  Held2
};

class CCords
{
  using ActionFunction = std::function<void()>;

public:
  CCords(uint8_t pin1, uint8_t pin2);
  ~CCords();

  void Handler();

  void SetAction(ECordsAction action, ActionFunction func);

protected:
  CCordSwitch mCord1;
  CCordSwitch mCord2;
  std::map<ECordsAction, ActionFunction> mActionMap;

  void RunAction(ECordsAction action);
};