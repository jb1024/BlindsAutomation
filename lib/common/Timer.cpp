#include "Timer.h"

#include "Platform.h"

CTimer::CTimer()
{
  Reset();
}

CTimer::~CTimer()
{
}

void CTimer::Reset()
{
  mStart = millis();
}

uint32_t CTimer::GetTime()
{
  return millis() - mStart;
}

bool CTimer::TimeOut(uint32_t period)
{
  if (GetTime() > period)
    return true;
  return false;
}

CTimer::operator uint32_t()
{
  return GetTime();
}
