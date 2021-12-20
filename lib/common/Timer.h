#pragma once

#include <cstdint>

class CTimer
{
public:
  CTimer();
  ~CTimer();

  void Reset();

  uint32_t GetTime();
  bool TimeOut(uint32_t period);

  operator uint32_t(void);

protected:
  uint32_t mStart = 0;
};