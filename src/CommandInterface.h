#pragma once

// The command interface opens a socket and listens for incoming commands
// Commands are executed immedately using the interfaces obtains form the system class

#include <WiFiNINA.h>
#include <stdint.h>
#include <string>

#include "Log.h"
#include "Parser.h"
#include "System.h"

class CCommandInterface : public Log::ILogger
{
public:
  CCommandInterface(uint16_t port);
  ~CCommandInterface();

  void OnLog(Log::ESeverity level, const std::string& msg) override;

  void Handler();

protected:
  CParser mParser;
  bool mStarted = false;
  WiFiServer mServer;
  WiFiClient mClient;
  std::string mReply;
  bool mErrors = false;
  // bool mLogEnabled = false;
  bool mConnected = false;

  std::string Parse();
  bool ShowHelp();
  bool Get();
  bool Set();

  bool SetPosition();
  bool SetPositionPreset();
  bool SetPositionRelative();
  bool SetPositionAbsolute();

  bool ProcessCommand(const std::string& command);

  CAxis& GetAxis();
  CStatus& GetStatus();
};