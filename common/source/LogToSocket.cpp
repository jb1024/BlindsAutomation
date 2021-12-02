#include "LogToSocket.h"

#include "Log.h"

CLogToSocket::CLogToSocket()
{
}

CLogToSocket::~CLogToSocket()
{
  Log::UnregisterLogger(*this);
}

void CLogToSocket::Enable(const SSocketAddress& sa)
{
  mIp = fmt::format("{}.{}.{}.{}", sa.IPAddress[0], sa.IPAddress[1], sa.IPAddress[2], sa.IPAddress[3]);
  mPort = sa.Port;
  if (sa.Port > 0)
  {
    mSocket.begin(5001);
    Log::RegisterLogger(*this);
  }
}

void CLogToSocket::OnLog(Log::ESeverity level, const std::string& msg)
{
  mSocket.beginPacket(mIp.c_str(), mPort);
  mSocket.write(msg.c_str());
  mSocket.write("\r\n");
  mSocket.endPacket();
}