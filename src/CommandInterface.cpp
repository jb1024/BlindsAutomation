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

CStatus& CCommandInterface::GetStatus()
{
  return CSystem::Get().GetStatus();
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

bool CCommandInterface::ShowHelp()
{
  mClient.write("Version: 0.0.5        : 0.0.5: Added LED override commands.\n");
  mClient.write("                      : 0.0.4: Just a first version.\n\n");

  mClient.write("reboot                : Reboots the system.\n\n");

  // Config space related commands
  mClient.write("load                  : Load configuration settings from storage.\n"); //@JB: Added the load command,
                                                                                        // but not sure why it is here?
  mClient.write("set hostname [value]  : Sets the device hostname name for wifi network.\n");
  mClient.write("                        In accesspoint mode the hostname is used as ssid.\n");
  mClient.write("get hostname          : Gets the device hostname.\n\n");

  mClient.write("set ssid [value]      : Sets name of the wifi network to connect with.\n");
  mClient.write("get ssid              : Gets name of the wifi network to connect with.\n");
  mClient.write("set password [value]  : Sets the password for the wifi network specified with ssid.\n\n");

  mClient.write("set udplogger [value] : Sets the socket address for udp logging.\n");
  mClient.write("                        Format is [ip]:[port]. For example: 10.0.0.30:5000.\n");
  mClient.write("                        UDP logging is disabled when the specified port is 0.\n");
  mClient.write("get udplogger         : Gets the socket address of the udp logger.\n\n");

  mClient.write("set speed [value]     : Sets maximum axis speed in degrees per second.\n");
  mClient.write("                        Minimum value = 1\n");
  mClient.write("get speed             : Gets the maximum axis speed in degrees per second.\n");

  mClient.write("set preset [value]    : Sets the specified preset to the current position.\n");
  mClient.write("                        Valid range: 1 .. 4\n");
  mClient.write("get preset            : Gets the position for the specified preset.\n\n");

  mClient.write("save                  : Save configuration changes.\n\n");
  // end config space related commands

  mClient.write("set pos [value]       : Sets the position of the axis.\n");
  mClient.write("                        0 .. 100 move to absolute position in percentage of the total stroke.\n");
  mClient.write("                        -100 .. +100 move relative in percentage of the total stroke.\n");
  mClient.write("                        p1 .. p4 move to preset position.\n");
  mClient.write("get pos               : Gets the current position of the axis.\n\n");

  mClient.write("set led1red            : Forces LED1 to red.\n");
  mClient.write("set led1green          : Forces LED1 to green.\n");
  mClient.write("set led1redgreen       : Forces LED1 to green.\n");
  mClient.write("set led1off            : Forces LED1 to red.\n");
  mClient.write("set led1release        : Releases LED1 to normal behavior.\n\n");

  return true;
}

bool CCommandInterface::Get()
{
  auto txt = Parse();

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

  if (txt == "speed")
  {
    mReply = fmt::format("{}", Config::GetSpeed());
    return true;
  }

  if (txt == "preset")
  {
    auto index = atoi(Parse().c_str());
    auto pos = Config::GetPreset(index);
    mReply = fmt::format("{}", pos);
    return true;
  }

  if (txt == "pos")
  {
    mReply = fmt::format("{}", GetAxis().GetPosition());
    return true;
  }

  Log::Error("Unexpected keyword: '{}'", txt);
  return false;
}

bool CCommandInterface::Set()
{
  auto txt = Parse();

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

  if (txt == "password")
  {
    Config::SetPassword(Parse());
    return true;
  }

  if (txt == "udplogger")
  {
    SSocketAddress sa;
    if (ToSocketAddress(Parse(), sa))
      Config::SetUdpLogger(sa);
    return true;
  }

  if (txt == "speed")
  {
    double speed = atof(Parse().c_str());
    CSystem::Get().GetAxis().SetSpeed(speed);
    Config::SetSpeed(speed);
    return true;
  }

  if (txt == "preset")
  {
    auto index = atoi(Parse().c_str());
    auto pos = GetAxis().GetPosition();
    Config::SetPreset(index, pos);
    return true;
  }

  if (txt == "pos")
    return SetPosition();

  if (txt == "led1red")
  {
    CSystem::Get().GetStatus().SetLed1Override(ELedOverride::ForceRed);
    return true;
  }

  if (txt == "led1green")
  {
    CSystem::Get().GetStatus().SetLed1Override(ELedOverride::ForceGreen);
    return true;
  }

  if (txt == "led1redgreen")
  {
    CSystem::Get().GetStatus().SetLed1Override(ELedOverride::ForceRedGreen);
    return true;
  }

  if (txt == "led1off")
  {
    CSystem::Get().GetStatus().SetLed1Override(ELedOverride::ForceOff);
    return true;
  }

  if (txt == "led1release")
  {
    CSystem::Get().GetStatus().SetLed1Override(ELedOverride::Release);
    return true;
  }
  Log::Error("Unexpected keyword: '{}'", txt);
  return false;
}

bool CCommandInterface::ProcessCommand(const string& command)
{
  mReply.clear();
  mErrors = false;

  if (command == "reboot")
    CSystem::Get().Reboot();

  if (command == "set")
    return Set();

  if (command == "get")
    return Get();

  if (command == "load")
    return Config::Load();

  if (command == "save")
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

      mParser.Init(buffer);
      auto command = Parse();

      if (command.length() == 0)
      {
        ShowHelp();
      }
      else
      {
        Log::Info("Command received: {}", buffer);

        ProcessCommand(command);

        if (mErrors)
          mClient.write("NOK\n");
        else
          mClient.write("OK\n");

        if (mReply.size() > 0)
        {
          mClient.write(mReply.c_str());
          mClient.write("\n");
        }
      }
      mClient.write("> ");
    }
  }
}
