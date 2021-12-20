#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

#include "SocketAddress.h"

namespace Interfaces
{

class ITcpSocket
{
public:
  virtual bool Connect(const std::string& ip, uint16_t port, double timeOut) = 0;
  virtual size_t Send(const void* data, size_t size) = 0;
  virtual size_t Receive(void* data, size_t size) = 0;
};

class IUdpSocket
{
public:
  virtual size_t SendTo(const uint8_t* data, size_t size, const std::string& ip, uint16_t port) = 0;
  virtual size_t ReceiveFrom(uint8_t* data, size_t size, std::string& ip, uint16_t& port) = 0;
};

} // namespace Interfaces