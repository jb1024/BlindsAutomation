#include "LogToSocket.h"

#include "Log.h"

CLogToSocket::CLogToSocket()
{
}

CLogToSocket::~CLogToSocket()
{
  Log::UnregisterLogger(*this);
}

void CLogToSocket::Enable(IPAddress ip, uint16_t port)
{
  mIp = fmt::format("{}.{}.{}.{}", ip[0], ip[1], ip[2], ip[3]);
  mPort = port;
  mSocket.begin(5001);
  Log::RegisterLogger(*this);
}

void CLogToSocket::OnLog(Log::ESeverity level, const std::string& msg)
{
  mSocket.beginPacket(mIp.c_str(), mPort);
  mSocket.write(msg.c_str());
  mSocket.write("\r\n");
  mSocket.endPacket();
}