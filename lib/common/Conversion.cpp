#include <cstring>
#include <fmt/format.h>

#include "Log.h"
#include "Parser.h"
#include "Socket.h"

using std::string;
using namespace Interfaces;

bool ToSocketAddress(const std::string& txt, SSocketAddress& sa)
{
  bool result = true;
  const char delimiters[] = "...:";
  string substr;

  CParser parser;
  parser.Init(txt);
  for (uint8_t i = 0; i < sizeof(sa.IPAddress); i++)
  {
    parser.While("0");
    parser.SubStr(substr);
    sa.IPAddress[i] = atoi(substr.c_str());

    if (parser.Look() != delimiters[i])
      result = false;
    parser.Skip();
  }
  parser.While("0");
  parser.SubStr(substr);
  sa.Port = atoi(substr.c_str());
  if (!parser.Ready())
    result = false;

  if (result == false)
    Log::Error("Specified string has wrong format. Expected: 'x.x.x.x:x', actual:{}", txt);

  return result;
}

std::string ToString(const SSocketAddress& sa)
{
  return fmt::format("{}.{}.{}.{}:{}", sa.IPAddress[0], sa.IPAddress[1], sa.IPAddress[2], sa.IPAddress[3], sa.Port);
}