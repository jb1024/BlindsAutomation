#pragma once

#include <cstdint>

#include <Pin.h>

const struct SPins
{
  SPin Led0 = {13, false};
  SPin Led1 = {3, true};
  SPin Led2 = {5, true};
  SPin Cord1 = {7, true};
  SPin Cord2 = {8, true};
  SPin Servo = {12, false};
} Pins;
