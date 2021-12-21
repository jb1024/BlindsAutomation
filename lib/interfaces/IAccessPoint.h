#pragma once

#include <string>

#include "SocketAddress.h"

namespace Interfaces
{

class IAccessPoint
{
public:
  // Connects to an accesspoint
  virtual bool Connect(const std::string& ssid, const std::string& password) = 0;

  // Creats an accesspoint
  virtual bool Create(const std::string& hostname, const std::string& ip) = 0;

  virtual void ReportStatus() = 0;
};

} // namespace Interfaces