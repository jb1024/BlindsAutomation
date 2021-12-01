#include "Config.h"

#include "Log.h"
#include "WiFiNINA.h"

#include <map>

using std::string;

namespace Config
{

using ConfigMap = std::map<std::string, std::string>;

#define CONFIG_FILE "/fs/config"

const uint8_t NumberOfPresets = 4;

struct SConfig
{
  char Hostname[40];
  char Ssid[40];
  char Password[40];
  IPAddress UDPLoggerIP;
  uint16_t UDPLoggerPort;
  double Presets[NumberOfPresets];
};

SConfig config;

bool Reset()
{
  memset(&config, 0, sizeof(config));
  strncpy(config.Hostname, "Arduino", 40);
  strncpy(config.Ssid, "", sizeof(config.Ssid));
  strncpy(config.Password, "", sizeof(config.Password));
  return Save();
}

bool Load()
{
  Log::Info("Loading configuration...");

  memset(&config, 0, sizeof(config));

  auto file = WiFiStorage.open(CONFIG_FILE);
  if (!file)
  {
    Log::Error("Config file could not be opened: {}", CONFIG_FILE);
    return false;
  }

  Log::Info("Reading configuration...");

  file.seek(0);
  auto todo = sizeof(config);
  auto amount = file.read(&config, sizeof(config));
  file.close();

  if (amount != todo)
  {
    Log::Warning("Config file has wrong size. Resetting to defaults...");
    return Reset();
  }

  return true;
}

bool Save()
{
  Log::Info("Saving configuration...");
  auto file = WiFiStorage.open(CONFIG_FILE);
  file.erase();

  auto todo = sizeof(config);
  auto amount = file.write(&config, todo);

  if (amount != todo)
  {
    file.erase();
    return false;
  }

  file.close();
  return true;
}

std::string GetHostName()
{
  return config.Hostname;
}

bool SetHostName(const string& hostname)
{
  if (hostname.length() == 0)
    return false;

  strncpy(config.Hostname, hostname.c_str(), sizeof(config.Hostname));
  return true;
}

std::string GetWifiSSID()
{
  return config.Ssid;
}

bool SetWifiSSID(const string& ssid)
{
  if (ssid.length() == 0)
    return false;

  strncpy(config.Ssid, ssid.c_str(), sizeof(config.Ssid));
  return true;
}

std::string GetWifiPassword()
{
  return config.Password;
}

void SetWifiPassword(const string& password)
{
  strncpy(config.Password, password.c_str(), sizeof(config.Password));
}

double GetPreset(uint8_t index)
{
  double value = index;
  value *= 20;
  return value;
}

} // namespace Config