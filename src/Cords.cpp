#include "Cords.h"
#include "System.h"

CCords::CCords(uint8_t pin1, uint8_t pin2)
    : mCord1(pin1)
    , mCord2(pin2)
{
}

CCords::~CCords()
{
}

void CCords::RunAction(ECordsAction action)
{
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

  if (mCord1.IsHeld() && mCord2.IsHeld())
    RunAction(ECordsAction::BothHeld);
}

void CCords::SetAction(ECordsAction action, const ActionFunction func)
{
  mActionMap[action] = func;
}
