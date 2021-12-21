#include <fmt/format.h>

#include "LogToSocket.h"

#include "Log.h"

using std::string;

CLogToSocket::CLogToSocket()
{
}

CLogToSocket::~CLogToSocket()
{
  Log::UnregisterLogger(*this);
}

void CLogToSocket::Enable(const SSocketAddress& sa)
{
  mIp = fmt::format("{}.{}.{}.{}", sa.IPAddress[0], sa.IPAddress[1], sa.IPAddress[2], sa.IPAddress[3]);
  mPort = sa.Port;
  if (sa.Port > 0)
  {
    mSocket.begin(5001);
    Log::RegisterLogger(*this);
    Log::Info("Logging to {}:{}", mIp, mPort);
  }
}

void CLogToSocket::Disable()
{
  Log::UnregisterLogger(*this);
}

string CLogToSocket::GetSeverityString(Log::ESeverity level)
{
  if (level == Log::ESeverity::Debug)
    return "";
  if (level == Log::ESeverity::Info)
    return "";
  if (level == Log::ESeverity::Warning)
    return "Warning: ";
  if (level == Log::ESeverity::Error)
    return "Error: ";
  if (level == Log::ESeverity::Fatal)
    return "Fatal: ";
  return "";
}

#define NO_COLOR = "\x1b\[0m"; // #Uncolored SERVER_COLOR = "\x1b\[31m" #Red

string CLogToSocket::GetSeverityColor(Log::ESeverity level)
{
  if (level == Log::ESeverity::Debug)
    return "\e[38;5;7m";
  if (level == Log::ESeverity::Info)
    return "\e[38;5;32m";
  if (level == Log::ESeverity::Warning)
    return "\e[38;5;208m";
  if (level == Log::ESeverity::Error)
    return "\e[38;5;196m";
  if (level == Log::ESeverity::Fatal)
    return "\e[48;5;196m";
  return "";
}

void CLogToSocket::OnLog(Log::ESeverity level, const std::string& msg)
{
  string txt;
  txt = fmt::format("{}{}{}\r{}\n", GetSeverityColor(level), GetSeverityString(level), msg, "\e[0m");

  mSocket.beginPacket(mIp.c_str(), mPort);
  mSocket.write(txt.c_str());
  mSocket.endPacket();
}