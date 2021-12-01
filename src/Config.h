#pragma once

#include <string>

namespace Config
{

bool Reset();
bool Load();
bool Save();

enum EConfigField
{
  HOSTNAME,
};

std::string GetString(EConfigField field);

std::string GetHostName();
bool SetHostName(const std::string& hostName);

std::string GetWifiSSID();
bool SetWifiSSID(const std::string& ssid);

std::string GetWifiPassword();
void SetWifiPassword(const std::string& password);

double GetPreset(uint8_t index);
void SetPreset(uint8_t index);

} // namespace Config
