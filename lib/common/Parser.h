#pragma once

#include <stdint.h>

#include <string>

class CParser
{
public:
  CParser(const std::string& txt = "");
  ~CParser(void);

  void Init(const std::string& txt);
  bool Ready(void); // Returns true if all characters are parsed.

  uint32_t While(const std::string& match); // Returns the amount of matching characters
  uint32_t Until(const std::string& match); // Returns the amount of non matching characters

  char Look(uint32_t offset = 0); // Returns character with offset Size from substring
  bool Skip(uint32_t size = 1);   // Increments the current position with the amount specified by Size
  bool Revert(uint32_t size = 1); // Decrements the current position with the amount specified by Size
  uint32_t GetLineNr(void);

  // After the While or Until function, the part which is parsed can be accessed using the following functions

  bool SubCompare(const std::string& txt); // Compares the substring with specified text
  uint32_t SubLen(void);                   // Is equal to the length returned by While/Until

  char SubChar(uint32_t pos = 0); // Returns character on position in parsed text
  void SubStr(std::string& txt);  // Copies the part that was parsed into a string referenced by rTxt

protected:
  std::string mTxt;           // pointer to string object holding the text to parse
  const std::string* mpMatch; // pointer to string object holding the characters to match
  uint32_t mSize;             // Size of text to parse
  uint32_t mLine;             // Line number (incremented after parsing an 0xA character)
  uint32_t mPos;              // Actual position in buffer
  uint32_t mSubPos;           // Start of last part that was parsed
  uint32_t mLen;              // Length of last part that was parsed

  bool Match(void);
  bool Next(void);     // Advances to next character
  bool Previous(void); // Backwards to previous character
};
