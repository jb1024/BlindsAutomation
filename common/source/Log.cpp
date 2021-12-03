#include "Log.h"

#include "Console.h"
#include <set>
#include <string>

namespace Log
{

using std::set;
using std::string;

std::set<ILogger*> Loggers;
uint32_t ErrorCount = 0;
string LastError;

void RegisterLogger(ILogger& logger)
{
  Loggers.insert(&logger);
}

void UnregisterLogger(ILogger& logger)
{
  Loggers.erase(&logger);
}

void Log(ESeverity severity, const std::string& message)
{
  char v[100];
  itoa(Loggers.size(), v, 10);
  Console::WriteLine(v);

  for (auto& logger : Loggers)
    logger->OnLog(severity, message);

  if (severity == ESeverity::Error)
  {
    LastError = message;
    ErrorCount++;
  }

  if (severity == ESeverity::Fatal)
    while (1)
    {
      // Stop process.
    };
}

} // namespace Log
