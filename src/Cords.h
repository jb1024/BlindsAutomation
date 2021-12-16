#pragma once

#include <cstdint>
#include <functional>
#include <map>

#include "CordSwitch.h"
#include "Timer.h"

enum class ECordsAction
{
  None,
  C1_Pull1,    // Cord 1 is pulled 1 time
  C1_Pull2,    // Cord 1 is pulled 2 times
  C1_Pull3,    // Cord 1 is pulled 3 times
  C1_Pull4,    // Cord 1 is pulled 4 times
  C1_Held,     // Cord 1 is held
  C1_Released, // Cord 1 is released (after held)
  C2_Pull1,    // Cord 2 is pulled 1 time
  C2_Pull2,    // Cord 2 is pulled 2 times
  C2_Pull3,    // Cord 2 is pulled 3 times
  C2_Pull4,    // Cord 2 is pulled 4 times
  C2_Held,     // Cord 2 is held
  C2_Released, // Cord 2 is released (after held)
  Cx_Held,     // Both cords are held
  Cx_Released  // Both cords are released (after held)
};

class CCords
{
  using ActionFunction = std::function<void()>;

  enum class EHeldState
  {
    Idle,         // Waiting for one of the cords to be held
    WaitForHeldx, // Waiting for both cords to be held
    Held1,        // Only cord 1 is held
    Held2,        // Only cord 2 is held
    Heldx,        // Both cords are held
  };

public:
  CCords(const SPin& pin1, const SPin& pin2);
  ~CCords();

  void Handler();

  void SetAction(ECordsAction action, ActionFunction func);

protected:
  CCordSwitch mCord1;
  CCordSwitch mCord2;
  CTimer mTimer;
  EHeldState mHeldState = EHeldState::Idle;
  std::map<ECordsAction, ActionFunction> mActionMap;

  ECordsAction GetHeldAction();
  ECordsAction GetPullAction();
  ECordsAction GetAction();
  void RunAction();
};