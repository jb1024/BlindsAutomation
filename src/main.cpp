//#include <Arduino.h>

#include "CommandInterface.h"
#include "System.h"

CCommandInterface ci(5000);

void setup()
{
  CSystem::Get().Initialize();
}

void loop()
{
  CSystem::Get().Handler();
  ci.Handler();
}