#pragma once

// Common
#include "LogToConsole.h"
#include "LogToSocket.h"

// Implementation
#include "Axis.h"
#include "Config.h"
#include "Cords.h"
#include "Status.h"

class CSystem
{
public:
  static CSystem& Get();

  void Initialize();
  void Handler();

  // System commands:
  void Reboot();

  // Interfaces:
  CStatus& GetStatus();
  CAxis& GetAxis();

protected:
  CSystem();
  ~CSystem();

  CLogToConsole mLogToConsole;
  CLogToSocket mLogToSocket;

  CStatus mStatus;
  CAxis mAxis;
  CCords mCords;

  bool ConnectToWifi();
  bool CreateAccessPoint();
};