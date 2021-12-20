#pragma once

#include <string>

#include "../../.pio/libdeps/nano/fmt/include/fmt/format.h"

namespace Log
{

enum class ESeverity
{
  Debug,
  Info,
  Warning,
  Error,
  Fatal
};

class ILogger
{
public:
  virtual void OnLog(ESeverity level, const std::string& msg) = 0;

protected:
  virtual ~ILogger(){};
};

void RegisterLogger(ILogger& logger);
void UnregisterLogger(ILogger& logger);
void Log(ESeverity severity, const std::string& message);

// Convenience functions
template <typename... Args> static void Debug(const std::string& format, Args&&... args)
{
  Log(ESeverity::Debug, fmt::format(format, std::forward<Args>(args)...));
}

template <typename... Args> static void Info(const std::string& format, Args&&... args)
{
  Log(ESeverity::Info, fmt::format(format, std::forward<Args>(args)...));
}

template <typename... Args> static void Warning(const std::string& format, Args&&... args)
{
  Log(ESeverity::Warning, fmt::format(format, std::forward<Args>(args)...));
}

template <typename... Args> static void Error(const std::string& format, Args&&... args)
{
  Log(ESeverity::Error, fmt::format(format, std::forward<Args>(args)...));
}

template <typename... Args> static void Fatal(const std::string& format, Args&&... args)
{
  Log(ESeverity::Fatal, fmt::format(format, std::forward<Args>(args)...));
}

} // namespace Log
