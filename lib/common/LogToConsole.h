#pragma once

#include "Log.h"

class CLogToConsole : public Log::ILogger
{
public:
  CLogToConsole();
  virtual ~CLogToConsole();

  void Enable();

  void OnLog(Log::ESeverity level, const std::string& msg) override;
};