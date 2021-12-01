#pragma once

#include "Axis.h"
#include "Leds.h"
#include "LogToConsole.h"
#include "LogToSocket.h"

class CSystem
{
public:
  static CSystem& Get();

  void Initialize();
  void Handler();

  void Reboot();

  CLeds& GetLeds();
  CAxis& GetAxis();

protected:
  CSystem();
  ~CSystem();

  CLogToConsole mLogToConsole;
  CLogToSocket mLogToSocket;
  CLeds mLeds;
  CAxis mAxis;

  bool ConnectToWifi();
  bool CreateAccessPoint();

  // CCords mCords;
};