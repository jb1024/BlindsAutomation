#pragma once

#include <WiFiNINA.h>
#include <memory>

#include "Log.h"
#include "Socket.h"

class CLogToSocket : public Log::ILogger
{
public:
  CLogToSocket();
  virtual ~CLogToSocket();

  void Enable(const SSocketAddress& sa);
  void Disable();

  void OnLog(Log::ESeverity level, const std::string& msg) override;

protected:
  WiFiUDP mSocket;
  std::string mIp;
  uint16_t mPort;

  std::string GetSeverityString(Log::ESeverity level);
  std::string GetSeverityColor(Log::ESeverity level);
};