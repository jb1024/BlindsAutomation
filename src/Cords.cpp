#include "Cords.h"
#include "System.h"

const uint32_t HeldWaitTime = 100;

CCords::CCords(const SPin& pin1, const SPin& pin2)
    : mCord1(pin1)
    , mCord2(pin2)
{
}

CCords::~CCords()
{
}

ECordsAction CCords::GetHeldAction()
{
  // Sample held states
  auto held1 = mCord1.IsHeld();
  auto held2 = mCord2.IsHeld();

  switch (mHeldState)
  {

  case EHeldState::Idle:
    // Wait until one of the cords to be held
    if (held1 || held2)
    {
      // If cord 1 is held, wait a short period for the other cord
      mHeldState = EHeldState::WaitForHeldx;
      mTimer.Reset(); // Reset the timer before waiting for the other cord
    }
    return ECordsAction::None;

  case EHeldState::WaitForHeldx:
    // Give the other cord some time to be held before the timeout
    if (held1 && held2) // Both cords are held?
    {
      mHeldState = EHeldState::Heldx;
      return ECordsAction::Cx_Held;
    }
    if (mTimer.TimeOut(HeldWaitTime))
    {
      if (held1) // Only cord 1?
      {
        mHeldState = EHeldState::Held1;
        return ECordsAction::C1_Held;
      }
      if (held2) // Only cord 2?
      {
        mHeldState = EHeldState::Held2;
        return ECordsAction::C2_Held;
      }
    }
    return ECordsAction::None;

  case EHeldState::Held1:
    if (held1 == false) // Cord 1 released?
    {
      mHeldState = EHeldState::Idle;
      return ECordsAction::C1_Released;
    }
    return ECordsAction::None;

  case EHeldState::Held2:
    if (held2 == false) // Cord 2 released?
    {
      mHeldState = EHeldState::Idle;
      return ECordsAction::C2_Released;
    }
    return ECordsAction::None;

  case EHeldState::Heldx:
    if ((held1 == false) && (held2 == false)) // Both cords released?
    {
      mHeldState = EHeldState::Idle;
      return ECordsAction::Cx_Released;
    }
    return ECordsAction::None;
  }

  return ECordsAction::None;
}

ECordsAction CCords::GetPullAction()
{
  uint8_t pulls1 = mCord1.GetNumberOfPulls();
  uint8_t pulls2 = mCord2.GetNumberOfPulls();

  if (pulls1 == 1)
    return ECordsAction::C1_Pull1;
  if (pulls1 == 2)
    return ECordsAction::C1_Pull2;
  if (pulls1 == 3)
    return ECordsAction::C1_Pull3;
  if (pulls1 == 4)
    return ECordsAction::C1_Pull4;

  if (pulls2 == 1)
    return ECordsAction::C2_Pull1;
  if (pulls2 == 2)
    return ECordsAction::C2_Pull2;
  if (pulls2 == 3)
    return ECordsAction::C2_Pull3;
  if (pulls2 == 4)
    return ECordsAction::C2_Pull4;

  return ECordsAction::None;
}

ECordsAction CCords::GetAction()
{
  auto action = GetPullAction();
  if (action != ECordsAction::None)
    return action;

  return GetHeldAction();
}

void CCords::RunAction()
{
  auto action = GetAction();
  if (action == ECordsAction::None)
    return;

  if (mActionMap.find(action) == mActionMap.end())
  {
    Log::Debug("No action defined for {}", action);
    return;
  }

  mActionMap[action]();
}

void CCords::Handler()
{
  mCord1.Handler();
  mCord2.Handler();
  RunAction();
}

void CCords::SetAction(ECordsAction action, const ActionFunction func)
{
  mActionMap[action] = func;
}
