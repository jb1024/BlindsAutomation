#pragma once

// The command interface opens a socket and listens for incoming commands
// Commands are executed immedately using the interfaces obtains form the system class

#include <WiFiNINA.h>
#include <stdint.h>
#include <string>

#include "Parser.h"
#include "System.h"

class CCommandInterface
{
public:
  CCommandInterface(uint16_t port);
  ~CCommandInterface();

  void Handler();

protected:
  CParser mParser;
  bool mStarted = false;
  WiFiServer mServer;
  WiFiClient mClient;
  std::string mReply;

  std::string Parse();
  bool Help();
  bool Get();

  bool Set();
  bool SetPosition();
  bool SetPositionPreset();
  bool SetPositionRelative();
  bool SetPositionAbsolute();
  bool SetHostName();

  bool ProcessCommand(const std::string& command);

  CAxis& GetAxis();
  CLeds& GetLeds();
};