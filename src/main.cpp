#include "CommandInterface.h"
#include "Config.h"
#include "System.h"

#include "Console.h"
#include "LogToConsole.h"
// CCommandInterface ci(5000);
#include <iostream>

CLogToConsole logToConsole;
void setup()
{
  Console::WaitForConnection(1000);

  logToConsole.Enable();

  Config::Load();

  Log::Info("Hostname: {}", Config::GetHostname());
  Log::Info("SSID: {}", Config::GetSsid());

  // Use code below to set your wifi credentials directly without using the accesspoint
  // Don't forget to remove this code before releasing.
  // Config::Load();
  // Config::SetSsid("ssid");
  // Config::SetPassword("password");
  // Config::SetUdpLogger({{10,0,0,30},5001});
  // Config::.... possibly add more configuration parameters here
  // Config::Save();

  // Extra comments....

  // CSystem::Get().Initialize();
}

void loop()
{

  // CSystem::Get().Handler();
  // ci.Handler();
}