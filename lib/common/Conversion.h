#pragma once

#include <cstdint>
#include <string>

#include "ISocket.h"

bool ToSocketAddress(const std::string& txt, SSocketAddress& sa);
std::string ToString(const SSocketAddress& sa);