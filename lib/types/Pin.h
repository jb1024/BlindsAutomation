#pragma once

#include <cstdint>

enum class EPinMode
{
  Normal,
  Inverted
};

struct SPin
{
  const uint8_t Pin;
  const EPinMode Mode;
};
