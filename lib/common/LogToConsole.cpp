#include "LogToConsole.h"

#include "Console.h"
#include "Log.h"

CLogToConsole::CLogToConsole()
{
}

CLogToConsole::~CLogToConsole()
{
  Log::UnregisterLogger(*this);
}

void CLogToConsole::Enable()
{
  Log::RegisterLogger(*this);
}

void CLogToConsole::Disable()
{
  Log::UnregisterLogger(*this);
}

void CLogToConsole::OnLog(Log::ESeverity level, const std::string& msg)
{
  Console::WriteLine(msg.c_str());
}
