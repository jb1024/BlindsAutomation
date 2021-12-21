#pragma once

// Typedef
#include <Pin.h>

const struct SPins
{
  SPin Led0 = {13, EPinMode::Normal};
  SPin Led1 = {3, EPinMode::Inverted};
  SPin Led2 = {5, EPinMode::Inverted};
  SPin Cord1 = {7, EPinMode::Inverted};
  SPin Cord2 = {8, EPinMode::Inverted};
  SPin Servo = {12, EPinMode::Normal};
} Pins;
