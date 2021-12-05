#include <map>

#include <WiFiNINA.h>

#include "Config.h"
#include "Log.h"
#include "Parser.h"

using std::string;

namespace Config
{

const string ConfigFile = "/fs/config";

enum class EField
{
  Illegal, // Only used to indicate an illegal conversion from string
  Hostname,
  Ssid,
  Password,
  UdpLogger,
  Speed,
  Preset1,
  Preset2,
  Preset3,
  Preset4,
};

using TConfigMap = std::map<EField, string>;

const TConfigMap FieldMap = {
    {EField::Hostname, "hostname"},   {EField::Ssid, "ssid"},       {EField::Password, "password"},
    {EField::UdpLogger, "udplogger"}, {EField::Speed, "speed"},     {EField::Preset1, "preset1"},
    {EField::Preset2, "preset2"},     {EField::Preset3, "preset3"}, {EField::Preset4, "preset4"}};

TConfigMap ConfigMap;

string ToString(EField field)
{
  for (auto& i : FieldMap)
  {
    if (i.first == field)
      return i.second;
  }
  return "";
}

EField ToField(const string& field)
{
  for (auto& i : FieldMap)
  {
    if (i.second == field)
      return i.first;
  }
  return EField::Illegal;
}

string GetHostname()
{
  return ConfigMap[EField::Hostname];
}

void SetHostname(const string& hostname)
{
  if (hostname.length() == 0)
    Log::Error("Hostname cannot be empty.");
  else
    ConfigMap[EField::Hostname] = hostname;
}

string GetSsid()
{
  return ConfigMap[EField::Ssid];
}

void SetSsid(const string& ssid)
{
  ConfigMap[EField::Ssid] = ssid;
}

string GetPassword()
{
  return ConfigMap[EField::Password];
}

void SetPassword(const string& password)
{
  ConfigMap[EField::Password] = password;
}

SSocketAddress GetUdpLogger()
{
  SSocketAddress sa;
  ToSocketAddress(ConfigMap[EField::UdpLogger], sa);
  return sa;
}

void SetUdpLogger(const SSocketAddress& logger)
{
  ConfigMap[EField::UdpLogger] = ToString(logger);
}

EField GetPresetIndex(uint8_t index)
{
  if (index == 1)
    return EField::Preset1;

  if (index == 2)
    return EField::Preset2;

  if (index == 3)
    return EField::Preset3;

  if (index == 4)
    return EField::Preset4;

  Log::Error("Value for preset is out of range.");
  return EField::Illegal;
}

double GetSpeed()
{
  return atof(ConfigMap[EField::Speed].c_str());
}

void SetSpeed(double value)
{
  ConfigMap[EField::Speed] = fmt::format("{}", value);
}

double GetPreset(uint8_t index)
{
  EField field = GetPresetIndex(index);

  if (field == EField::Illegal)
    return 0;

  return atof(ConfigMap[field].c_str());
}

void SetPreset(uint8_t index, double value)
{
  EField field = GetPresetIndex(index);

  if (field == EField::Illegal)
    return;

  ConfigMap[field] = fmt::format("{}", value);
}

void ParseConfiguration(const string& content)
{
  CParser parser;
  parser.Init(content);
  while (!parser.Ready())
  {
    parser.Until("=");
    string name;
    parser.SubStr(name);
    parser.Skip();
    parser.Until("\r\n");
    string value;
    parser.SubStr(value);
    auto field = ToField(name);
    if (field == EField::Illegal)
    {
      Log::Warning("Ignoring illegal configuration parameter: {}={}", name, value);
    }
    else
    {
      ConfigMap[field] = value;
    }
    parser.While("\r\n");
  }
}

void Reset()
{
  ConfigMap.clear();
  SetHostname("Arduino");
}

bool Load()
{
  Log::Info("Loading configuration...");

  Reset();

  auto file = WiFiStorage.open(ConfigFile.c_str());
  if (!file)
  {
    Log::Warning("Config file could not be opened, reverting to defaults.");
    return false;
  }

  string content;

  char buffer[100];

  while (file.available())
  {
    auto amount = file.read(buffer, sizeof(buffer));
    content.append(buffer, amount);
  }

  Log::Info("{} bytes read.", content.size());
  ParseConfiguration(content);
  return true;
}

bool Save()
{
  Log::Info("Saving configuration...");

  string content;
  for (auto& i : ConfigMap)
    content.append(fmt::format("{}={}\n", ToString(i.first), i.second));

  auto file = WiFiStorage.open(ConfigFile.c_str());
  file.erase();

  auto amount = file.write(content.data(), content.size());
  if (amount != content.size())
  {
    Log::Error("Unable to write configuration file.");
    file.erase();
    return false;
  }
  file.close();

  Log::Info("{} bytes written.", content.size());
  return true;
}

} // namespace Config
