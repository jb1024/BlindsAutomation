#pragma once

#include "IAccessPoint.h"

class CAccessPoint : public Interfaces::IAccessPoint
{
public:
  CAccessPoint();
  ~CAccessPoint();

  // IAccessPoint implementation
  bool Connect(const std::string& ssid, const std::string& password) override;
  bool Create(const std::string& hostname, const std::string& ip) override;
  void ReportStatus() override;
};