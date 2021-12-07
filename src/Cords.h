#pragma once

#include <cstdint>
#include <functional>
#include <map>

#include "CordSwitch.h"

// C1_Held
// C2_Held
// Cx_Held
// C1_Pull1
// C1_Pull2
// C1_Pull3
// C1_Pull4
// C2_Pull1
// C2_Pull2
// C2_Pull3
// C2_Pull4

enum class ECordsAction
{
  None,
  C1_Pull1,
  C2_Pull1,
  C1_Pull2,
  C2_Pull2,
  C1_Pull3,
  C2_Pull3,
  C1_Pull4,
  C2_Pull4,
  C1_Held,
  C2_Held,
  Cx_Held
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

  ECordsAction GetAction();
  void RunAction();
};