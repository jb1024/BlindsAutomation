#include "Log.h"

#include "Pwm.h"

CPwm::CPwm(const SPin& pin)
{
}

CPwm::~CPwm()
{
}

void CPwm::SetFrequency(double Hz)
{
  Log::Fatal("Function is not supported yet");
}

void CPwm::SetPulseWidth(double percent)
{
}
