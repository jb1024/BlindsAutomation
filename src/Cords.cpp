#include "Cords.h"
#include "System.h"

CCords::CCords(const SPin& pin1, const SPin& pin2)
    : mCord1(pin1)
    , mCord2(pin2)
{
}

CCords::~CCords()
{
}

ECordsAction CCords::GetAction()
{
  uint8_t pulls1 = mCord1.GetNumberOfPulls();
  uint8_t pulls2 = mCord2.GetNumberOfPulls();
  bool held1 = mCord1.IsHeld();
  bool held2 = mCord2.IsHeld();
  bool releasedAfterHeld1 = mCord1.IsReleasedAdterHeld();
  bool releasedAfterHeld2 = mCord2.IsReleasedAdterHeld();

  if (held1 && held2)
    return ECordsAction::Cx_Held;
  if (held1)
    return ECordsAction::C1_Held;
  if (held2)
    return ECordsAction::C2_Held;
  if (releasedAfterHeld1)
    return ECordsAction::C1_HeldReleased;
  if (releasedAfterHeld2)
    return ECordsAction::C2_HeldReleased;
  // RVC: CxHeldReleased will not work perfect since the timing between releasing both cords will not be sampled at the
  // same time
  if (releasedAfterHeld1 && releasedAfterHeld2)
    return ECordsAction::Cx_HeldReleased;

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
