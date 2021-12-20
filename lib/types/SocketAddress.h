#pragma once

#include <cstdint>

struct SSocketAddress
{
  uint8_t IPAddress[4];
  uint16_t Port;
};
