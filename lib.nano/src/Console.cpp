#include <Arduino.h>

#include "Console.h"
#include "Log.h"

namespace Console
{

void Init()
{
  Serial.begin(9600);
}

bool WaitForConnection(double timeOut)
{
  while (timeOut > 0)
  {
    if (Serial)
    {
      Serial.print("Connected.");
      return true;
    }
    delay(100);
    timeOut -= 0.1;
  }

  Log::Warning("Timeout waiting for serial connection.");
  return false;
}

void Write(const std::string& message)
{
  Serial.print(message.c_str());
}

void WriteLine(const std::string& message)
{
  Serial.println(message.c_str());
}

} // namespace Console
