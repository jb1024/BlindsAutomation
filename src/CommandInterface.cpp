
#include "CommandInterface.h"
#include "Config.h"
#include "Log.h"
#include "System.h"

using std::string;

CCommandInterface::CCommandInterface(uint16_t port)
    : mServer(port)
{
  mReply.reserve(1500);
}

CCommandInterface::~CCommandInterface()
{
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

bool CCommandInterface::Help()
{
  mReply.append("-------------------\n");
  mReply.append("help               : Shows this text.\n");
  mReply.append("reboot             : Reboots the system.\n");
  mReply.append("set pos [value]    : Sets the position of the axis.\n");
  mReply.append("                     0 .. 100 move to absolute position in percentage of the total stroke.\n");
  mReply.append("                     -100 .. +100 move relative in percentage of the total stroke.\n");
  mReply.append("                     p1 .. p4 move to preset position.\n");
  mReply.append("set preset [value] : Saves the current position to specified preset.\n");
  mReply.append("                     Valid range: p1 .. p4\n");
  mReply.append("save               : Save configuration changes.");
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
    mReply = Config::GetHostName();
    return true;
  }

  if (txt == "ssid")
  {
    mReply = Config::GetWifiSSID();
    return true;
  }

  mReply = fmt::format("Unexpected keyword: '{}'", txt);
  return false;
}

bool CCommandInterface::Set()
{
  auto txt = Parse();

  if (txt == "pos")
    return SetPosition();

  if (txt == "hostname")
    return SetHostName();

  if (txt == "ssid")
    return Config::SetWifiSSID(Parse());

  mReply = fmt::format("Unexpected keyword: '{}'", txt);
  return false;
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

  GetLeds().SetSequence(ELedMode::Red, ELedMode::Red);
  GetAxis().MoveAbsolute(pos);
  delay(100);
  GetLeds().SetSequence(ELedMode::Off, ELedMode::Off);

  return true;
}

bool CCommandInterface::SetPositionRelative()
{
  auto txt = Parse();
  auto value = atof(txt.c_str());

  GetLeds().SetSequence(ELedMode::Red, ELedMode::Red);
  GetAxis().MoveRelative(value);
  delay(100);
  GetLeds().SetSequence(ELedMode::Off, ELedMode::Off);
  return true;
}

bool CCommandInterface::SetPositionAbsolute()
{
  auto txt = Parse();
  auto value = atof(txt.c_str());

  GetLeds().SetSequence(ELedMode::Red, ELedMode::Red);
  GetAxis().MoveAbsolute(value);
  delay(100);
  GetLeds().SetSequence(ELedMode::Off, ELedMode::Off);

  return true;
}

bool CCommandInterface::SetHostName()
{
  auto txt = Parse();
  Config::SetHostName(txt);
  return true;
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

  if (cmd == "save")
    return Config::Save();

  return false;
}

void CCommandInterface::Handler()
{
  if (!mStarted)
  {
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

      if (!ProcessCommand(buffer))
        mClient.write("NOK\n");
      else
        mClient.write("OK\n");

      mClient.write(mReply.c_str());
      mClient.write("\n");
    }
  }
}
