#pragma once

#include <cstdint>
#include <string>

struct SSocketAddress
{
  uint8_t IPAddress[4];
  uint16_t Port;
};

bool ToSocketAddress(const std::string& txt, SSocketAddress& sa);
std::string ToString(const SSocketAddress& sa);