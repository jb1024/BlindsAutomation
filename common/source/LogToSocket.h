#pragma once

#include <WiFiNINA.h>
#include <memory>

#include "Log.h"

class CLogToSocket : public Log::ILogger
{
public:
  CLogToSocket();
  virtual ~CLogToSocket();

  void Enable(IPAddress ip, uint16_t port);

  void OnLog(Log::ESeverity level, const std::string& msg) override;

protected:
  WiFiUDP mSocket;
  std::string mIp;
  uint16_t mPort;
};