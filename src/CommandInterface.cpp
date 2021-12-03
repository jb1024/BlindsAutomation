#include "Config.h"
#include "Log.h"

#include "CommandInterface.h"

using std::string;

CCommandInterface::CCommandInterface(uint16_t port)
    : mServer(port)
{
  mReply.reserve(1500);
}

CCommandInterface::~CCommandInterface()
{
}

void CCommandInterface::OnLog(Log::ESeverity level, const std::string& msg)
{
  if (level == Log::ESeverity::Error)
  {
    mReply.append(msg);
    mReply.append("\n");
    mErrors = true;
  }
}

CAxis& CCommandInterface::GetAxis()
{
  return CSystem::Get().GetAxis();
}

CLeds& CCommandInterface::GetLeds()
{
  return CSystem::Get().GetLeds();
}

string CCommandInterface::Parse()
{
  string txt;
  mParser.While(" \t\r\n");
  mParser.Until(" \t\r\n");
  mParser.SubStr(txt);
  return txt;
}

bool CCommandInterface::SetPosition()
{
  mParser.While(" \t\r\n");

  if (mParser.Look() == 'p')
    return SetPositionPreset();

  if ((mParser.Look() == '+') || (mParser.Look() == '-'))
    return SetPositionRelative();

  return SetPositionAbsolute();
}

bool CCommandInterface::SetPositionPreset()
{
  mParser.Skip(); // 'p'
  mParser.Until(" \t\r\n");
  string txt;
  mParser.SubStr(txt);
  auto index = atoi(txt.c_str());
  auto pos = Config::GetPreset(index);

  GetAxis().MoveAbsolute(pos);
  delay(100);

  return true;
}

bool CCommandInterface::SetPositionRelative()
{
  auto txt = Parse();
  auto value = atof(txt.c_str());

  GetAxis().MoveRelative(value);
  delay(100);
  return true;
}

bool CCommandInterface::SetPositionAbsolute()
{
  auto txt = Parse();
  auto value = atof(txt.c_str());

  GetAxis().MoveAbsolute(value);
  delay(100);

  return true;
}

bool CCommandInterface::Help()
{
  mReply.append("-------------------\n");
  mReply.append("help               : Shows this text.\n");
  mReply.append("reboot             : Reboots the system.\n");
  mReply.append("set pos [value]    : Sets the position of the axis.\n");
  mReply.append("                     0 .. 100 move to absolute position in percentage of the total stroke.\n");
  mReply.append("                     -100 .. +100 move relative in percentage of the total stroke.\n");
  mReply.append("                     p1 .. p4 move to preset position.\n");
  mReply.append("set preset [value] : Sets the current position to specified preset.\n");
  mReply.append("                     Valid range: 1 .. 4\n");
  mReply.append("save               : Save configuration changes.\n");
  mReply.append("\n");
  return true;
}

bool CCommandInterface::Get()
{
  auto txt = Parse();

  if (txt == "pos")
  {
    mReply = fmt::format("{}", GetAxis().GetPosition());
    return true;
  }

  if (txt == "hostname")
  {
    mReply = Config::GetHostname();
    return true;
  }

  if (txt == "ssid")
  {
    mReply = Config::GetSsid();
    return true;
  }

  if (txt == "udplogger")
  {
    mReply = ToString(Config::GetUdpLogger());
    return true;
  }

  if (txt == "preset")
  {
    auto index = atoi(Parse().c_str());
    auto pos = Config::GetPreset(index);
    mReply = fmt::format("{}", pos);
    return true;
  }

  Log::Error("Unexpected keyword: '{}'", txt);
  return false;
}

bool CCommandInterface::Set()
{
  auto txt = Parse();

  if (txt == "pos")
    return SetPosition();

  if (txt == "hostname")
  {
    Config::SetHostname(Parse());
    return true;
  }

  if (txt == "ssid")
  {
    Config::SetSsid(Parse());
    return true;
  }

  if (txt == "udplogger")
  {
    SSocketAddress sa;
    if (ToSocketAddress(Parse(), sa))
      Config::SetUdpLogger(sa);
    return true;
  }

  if (txt == "preset")
  {
    auto index = atoi(Parse().c_str());
    auto pos = GetAxis().GetPosition();
    Config::SetPreset(index, pos);
    return true;
  }

  Log::Error("Unexpected keyword: '{}'", txt);
  return false;
}

bool CCommandInterface::ProcessCommand(const string& command)
{
  mParser.Init(command);

  auto cmd = Parse();

  if (cmd == "help")
    return Help();

  if (cmd == "reboot")
    CSystem::Get().Reboot();

  if (cmd == "set")
    return Set();

  if (cmd == "get")
    return Get();

  if (cmd == "load")
    return Config::Load();

  if (cmd == "save")
    return Config::Save();

  Log::Error("Illegal command: {}", command);
  return false;
}

void CCommandInterface::Handler()
{
  if (!mStarted)
  {
    Log::RegisterLogger(*this);
    mServer.begin();
    mStarted = true;
  }

  mClient = mServer.available();
  std::string buffer;
  buffer.reserve(100); // Prevent reallocation on each added character

  if (mClient)
  {
    if (mClient.connected())
    {
      while (mClient.available())
      {
        buffer += mClient.read();
      }
      Log::Info("Command received: {}", buffer);

      mReply.clear();
      mErrors = false;

      if (!ProcessCommand(buffer))
        mErrors = true;

      if (mErrors)
        mClient.write("NOK\n");
      else
        mClient.write("OK\n");

      if (mReply.size() > 0)
      {
        mClient.write(mReply.c_str());
        mClient.write("\n");
      }
      mClient.write("> ");
    }
  }
}
