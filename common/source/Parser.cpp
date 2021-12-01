#include "Parser.h"

using std::string;

#define NEWLINE 0xA

CParser::CParser(const string& txt)
{
  Init(txt);
}

CParser::~CParser(void)
{
}

void CParser::Init(const std::string& txt)
{
  mTxt = txt;
  mSize = mTxt.length();
  mLine = 1; // Reset line number to 1
  mPos = 0;  // Set parsing position to 0 (first character)
  mSubPos = 0;
  mLen = 0;
}

bool CParser::Ready(void)
{
  if (mPos >= mSize)
    return (true);
  return (false);
}

uint32_t CParser::While(const string& match)
{
  mLen = 0;
  mSubPos = mPos;

  if (mPos >= mSize)
    return (mLen);

  mpMatch = &match;

  while (Match() == true)
  {
    if (Next() == false)
      return (mLen);
  }

  return (mLen);
}

uint32_t CParser::Until(const string& match)
{
  mLen = 0;
  mSubPos = mPos;

  if (mPos >= mSize)
    return (mLen);

  mpMatch = &match;

  while (Match() == false)
  {
    if (Next() == false)
      return (mLen);
  }

  return (mLen);
}

char CParser::Look(uint32_t offset)
{
  uint32_t pos;

  pos = mPos + offset;

  if (pos >= mSize)
    return (0);
  return (mTxt[pos]);
}

bool CParser::Skip(uint32_t size)
{
  for (uint32_t cnt = 0; cnt < size; cnt++)
  {
    if (Next() == false)
      return (false);
  }

  return (true);
}

bool CParser::Revert(uint32_t size)
{
  for (uint32_t cnt = 0; cnt < size; cnt++)
  {
    if (Previous() == false)
      return (false);
  }

  return (true);
}

uint32_t CParser::GetLineNr(void)
{
  return (mLine);
}

uint32_t CParser::SubLen(void)
{
  return (mLen);
}

char CParser::SubChar(uint32_t pos)
{
  uint32_t i;

  i = mSubPos + pos;
  if (i >= mSize)
    return (0);
  return (mTxt[i]);
}

bool CParser::SubCompare(const string& txt)
{
  if (mTxt.compare(mSubPos, mLen, txt) != 0)
  {
    return (false);
  }

  return (true);
}

void CParser::SubStr(string& txt)
{
  txt = mTxt.substr(mSubPos, mLen);
}

bool CParser::Match(void)
{
  uint32_t Cnt;
  uint32_t Size = mpMatch->length();
  char c = mTxt[mPos];
  char m;

  for (Cnt = 0; Cnt < Size; Cnt++)
  {
    m = mpMatch->at(Cnt);

    if (m == c)
      return (true);

    if (m == 'A')
    {
      if ((c >= 'A') && (c <= 'Z'))
        return (true);
    }

    if (m == 'a')
    {
      if ((c >= 'a') && (c <= 'z'))
        return (true);
    }

    if (m == '0')
    {
      if ((c >= '0') && (c <= '9'))
        return (true);
    }
  }

  return (false);
}

bool CParser::Next(void)
{
  mPos++;
  mLen++;

  if (mPos >= mSize)
    return (false);
  if (mTxt[mPos] == NEWLINE)
  {
    mLine++; // After linefeed, next line will be parsed
  }

  return (true);
}

bool CParser::Previous(void)
{
  if (mPos == 0)
    return false;
  if (mTxt[mPos] == NEWLINE)
  {
    mLine--;
  }
  mPos--;
  return true;
}
