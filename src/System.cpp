#include <Arduino.h>

#include "WiFiNINA.h"

#include "Config.h"
#include "Log.h"
#include "System.h"

CSystem& CSystem::Get()
{
  static CSystem instance;
  return instance;
}

CSystem::CSystem()
    : mLeds(3, 5)
    , mAxis(12)
{
}

CSystem::~CSystem()
{
}

bool CSystem::ConnectToWifi()
{
  auto ssid = Config::GetSsid();
  auto password = Config::GetPassword();

  if (ssid.length() == 0)
  {
    Log::Warning("Could not connect to existing wifi network because SSID is not set.");
    return false;
  }

  Log::Info("Connecting to existing wifi network...");
  auto status = WiFi.begin(ssid.c_str(), password.c_str());
  if (status != WL_CONNECTED)
  {
    mLogToConsole.Enable();
    Log::Warning("Unable to connect to wifi network: '{}'", ssid);
    return false;
  }

  mLogToSocket.Enable(Config::GetUdpLogger());

  auto ip = WiFi.localIP();
  Log::Info("Succesfully connected to wifi network.");
  Log::Info("IP: {}.{}.{}.{}", ip[0], ip[1], ip[2], ip[3]);
  return true;
}

bool CSystem::CreateAccessPoint()
{
  Log::Info("Creating access point...");
  WiFi.config(IPAddress(10, 0, 0, 1));
  auto status = WiFi.beginAP(Config::GetHostname().c_str());
  if (status != WL_AP_LISTENING)
  {
    Log::Error("Unable to create access point.");
    return false;
  }

  Log::Info("Access point created.");
  Log::Info("Goto http://10.0.0.1 for device configuration.");
  return true;
}

void CSystem::Initialize()
{
  Config::Load();

  mAxis.SetSpeed(Config::GetSpeed());

  mLeds.SetDelay(500);
  mLeds.SetSequence(ELedMode::Red, ELedMode::Red);

  auto hostname = Config::GetHostname();
  WiFi.setHostname(hostname.c_str());

  if (ConnectToWifi())
  {
    mLeds.SetSequence(ELedMode::Off, ELedMode::Off);
    return;
  }

  if (CreateAccessPoint())
  {
    mLeds.SetSequence(ELedMode::Red, ELedMode::Off);
    return;
  }

  mLeds.SetSequence(ELedMode::Red, ELedMode::Red);
  Reboot();
}

void CSystem::Handler()
{
  mLeds.Handler();
  mAxis.Handler();
}

void CSystem::Reboot()
{
  Log::Warning("Rebooting...");
  // Enable watchdog en wait forever
  REG_WDT_CTRL = WDT_CONFIG_PER_128;
  REG_WDT_CTRL = WDT_CTRL_ENABLE;
  while (1)
    ;
}

CLeds& CSystem::GetLeds()
{
  return mLeds;
}

CAxis& CSystem::GetAxis()
{
  return mAxis;
}
