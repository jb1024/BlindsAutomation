#pragma once

#include "ISocket.h"

class CTcpSocket : public Interfaces::ITcpSocket
{
public:
  bool Connect(const std::string& ip, uint16_t port, double timeOut);
  size_t Send(const void* data, size_t size) = 0;
  size_t Receive(void* data, size_t size) = 0;
};

class IUdpSocket : public Interfaces::IUdpSocket
{
public:
  virtual size_t SendTo(const uint8_t* data, size_t size, const std::string& ip, uint16_t port) = 0;
  virtual size_t ReceiveFrom(uint8_t* data, size_t size, std::string& ip, uint16_t& port) = 0;
};
