#pragma once

#include <string>

#include "SocketAddress.h"

namespace Config
{

bool Load();
bool Save();

std::string GetHostname();
void SetHostname(const std::string& hostname);

std::string GetSsid();
void SetSsid(const std::string& ssid);

std::string GetPassword();
void SetPassword(const std::string& password);

SSocketAddress GetUdpLogger();
void SetUdpLogger(const SSocketAddress& logger);

double GetSpeed();
void SetSpeed(double value);

double GetPreset(uint8_t index);
void SetPreset(uint8_t index, double value);

} // namespace Config
