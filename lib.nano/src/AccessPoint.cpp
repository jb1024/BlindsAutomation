#include "Log.h"

#include "AccessPoint.h"
#include "Conversion.h"

#include "WiFiNINA.h"

CAccessPoint::CAccessPoint()
{
}

CAccessPoint::~CAccessPoint()
{
}

bool CAccessPoint::Connect(const std::string& ssid, const std::string& password)
{
  Log::Info("Connecting to accesspoint {}...", ssid);
  auto status = WiFi.begin(ssid.c_str(), password.c_str());
  if (status != WL_CONNECTED)
  {
    Log::Warning("Unable to connect to wifi network: '{}'", ssid);
    return false;
  }
  Log::Info("Succesfully connected to wifi network.");
  ReportStatus();
  return true;
}

bool CAccessPoint::Create(const std::string& hostname, const std::string& ip)
{
  Log::Info("Creating access point...");

  IPAddress ipAddress;
  if (!ipAddress.fromString(ip.c_str()))
  {
    Log::Error("Illegal ip address specified: {}", ip);
    return false;
  }

  WiFi.config(ipAddress);
  auto status = WiFi.beginAP(hostname.c_str());
  if (status != WL_AP_LISTENING)
  {
    Log::Error("Unable to create access point.");
    return false;
  }

  Log::Info("Accesspoint successfully created.");
  ReportStatus();
  return true;
}

void CAccessPoint::ReportStatus()
{
  auto ip = WiFi.localIP();
  Log::Info("IP: {}.{}.{}.{}", ip[0], ip[1], ip[2], ip[3]);
  Log::Info("RSSI: {} dBm", WiFi.RSSI());
}
