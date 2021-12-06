#pragma once

#include "Axis.h"
#include "Config.h"
#include "Cords.h"
#include "Leds.h"
#include "LogToConsole.h"
#include "LogToSocket.h"

class CSystem
{
public:
  static CSystem& Get();

  void Initialize();
  void Handler();

  // System commands:
  void Reboot();

  // Interfaces:
  CLeds& GetLeds();
  CAxis& GetAxis();

protected:
  CSystem();
  ~CSystem();

  CLogToConsole mLogToConsole;
  CLogToSocket mLogToSocket;

  CLeds mLeds;
  CAxis mAxis;
  CCords mCords;

  bool ConnectToWifi();
  bool CreateAccessPoint();
};