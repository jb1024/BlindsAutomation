// Common
#include "Log.h"

// Implementation
#include "Config.h"
#include "PinAssignments.h"

// Self
#include "System.h"

CSystem& CSystem::Get()
{
  static CSystem instance;
  return instance;
}

CSystem::CSystem()
    : mStatus({Pins.Led0, Pins.Led1, Pins.Led2})
    , mAxis(Pins.Servo)
    , mCords(Pins.Cord1, Pins.Cord2)
{
  /* Code below is for debugging all possible cord actions:

    mCords.SetAction(ECordsAction::C1_Pull1, [&]() { Log::Debug("C1_Pull1"); });
    mCords.SetAction(ECordsAction::C1_Pull2, [&]() { Log::Debug("C1_Pull2"); });
    mCords.SetAction(ECordsAction::C1_Pull3, [&]() { Log::Debug("C1_Pull3"); });
    mCords.SetAction(ECordsAction::C1_Pull4, [&]() { Log::Debug("C1_Pull4"); });
    mCords.SetAction(ECordsAction::C1_Held, [&]() { Log::Debug("C1_Held"); });
    mCords.SetAction(ECordsAction::C1_Released, [&]() { Log::Debug("C1_Released"); });

    mCords.SetAction(ECordsAction::C2_Pull1, [&]() { Log::Debug("C2_Pull1"); });
    mCords.SetAction(ECordsAction::C2_Pull2, [&]() { Log::Debug("C2_Pull2"); });
    mCords.SetAction(ECordsAction::C2_Pull3, [&]() { Log::Debug("C2_Pull3"); });
    mCords.SetAction(ECordsAction::C2_Pull4, [&]() { Log::Debug("C2_Pull4"); });
    mCords.SetAction(ECordsAction::C2_Held, [&]() { Log::Debug("C2_Held"); });
    mCords.SetAction(ECordsAction::C2_Released, [&]() { Log::Debug("C2_Released"); });

    mCords.SetAction(ECordsAction::Cx_Held, [&]() { Log::Debug("Cx_Held"); });
    mCords.SetAction(ECordsAction::Cx_Released, [&]() { Log::Debug("Cx_Released"); });
  */

  mCords.SetAction(ECordsAction::Cx_Held, [&]() { Reboot(); });
  mCords.SetAction(ECordsAction::C1_Held, [&]() { mAxis.MoveAbsolute(0); });
  mCords.SetAction(ECordsAction::C1_Released, [&]() { mAxis.Stop(); });
  mCords.SetAction(ECordsAction::C2_Held, [&]() { mAxis.MoveAbsolute(100); });
  mCords.SetAction(ECordsAction::C2_Released, [&]() { mAxis.Stop(); });

  // Set preset 1 if cord 1 or 2 is pulled once
  {
      //  auto f = [&]() { mAxis.MoveAbsolute(Config::GetPreset(1)); };
      //  mCords.SetAction(ECordsAction::C1_Pull1, f);
      //  mCords.SetAction(ECordsAction::C2_Pull1, f);
  }

  // Set preset 2 if cord 1 or 2 is pulled twice
  {
    auto f = [&]() { mAxis.MoveAbsolute(Config::GetPreset(2)); };
    mCords.SetAction(ECordsAction::C1_Pull2, f);
    mCords.SetAction(ECordsAction::C2_Pull2, f);
  }

  {
    auto f = [&]() { mAxis.MoveAbsolute(Config::GetPreset(3)); };
    mCords.SetAction(ECordsAction::C1_Pull3, f);
    mCords.SetAction(ECordsAction::C2_Pull3, f);
  }

  {
    auto f = [&]() { mAxis.MoveAbsolute(Config::GetPreset(4)); };
    mCords.SetAction(ECordsAction::C1_Pull4, f);
    mCords.SetAction(ECordsAction::C2_Pull4, f);
  }
}

CSystem::~CSystem()
{
}

bool CSystem::ConnectToWifi()
{
  auto ssid = Config::GetSsid();
  auto password = Config::GetPassword();

  // mLogToConsole.Enable(); // Enable logging to console since Wifi is not (yet) available. But if I do this the
  // logging to the socket does not work anymore? Cannot log to both, or better, how to switch over to socket logging?
  // Test this by corrupting the SSID in the config
  // Where does the '0' and '1' logging come from? Cannot find it....
  if (ssid.length() == 0)
  {
    Log::Warning("Could not connect to existing wifi network because SSID is not set.");
    return false;
  }

  Log::Info("Connecting to existing wifi network...");
  auto status = WiFi.begin(ssid.c_str(), password.c_str());
  if (status != WL_CONNECTED)
  {
    Log::Warning("Unable to connect to wifi network: '{}'", ssid);
    return false;
  }

  mLogToSocket.Enable(Config::GetUdpLogger()); // Now that Wifi is enabled start logging to socket

  auto ip = WiFi.localIP();
  Log::Info("Succesfully connected to wifi network.");
  Log::Info("IP: {}.{}.{}.{}", ip[0], ip[1], ip[2], ip[3]);
  Log::Info("RSSI: {} dBm", WiFi.RSSI());
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

  mStatus.SetAccessPoint(true);
  Log::Info("Access point created.");
  Log::Info("Goto http://10.0.0.1 for device configuration.");
  return true;
}

void CSystem::Initialize()
{
  mStatus.SetBooting(true);

  Config::Load();

  mAxis.SetSpeed(Config::GetSpeed());

  auto hostname = Config::GetHostname();
  WiFi.setHostname(hostname.c_str());

  if (!ConnectToWifi())
  {
    if (!CreateAccessPoint())
      Reboot();
  }

  mStatus.SetBooting(false);
}

void CSystem::Handler()
{
  mStatus.Handler();
  mAxis.Handler();
  mCords.Handler();
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

CStatus& CSystem::GetStatus()
{
  return mStatus;
}

CAxis& CSystem::GetAxis()
{
  return mAxis;
}
