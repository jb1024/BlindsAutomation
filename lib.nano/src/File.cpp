#include "File.h"

using std::string;

CFile::CFile(const std::string& filename)
    : mFilePath(GetFilePath(filename))
    , mFile(mFilePath.c_str())
{
}

CFile::~CFile()
{
  Close();
}

string CFile::GetFilePath(const std::string& filename)
{
  string filePath = "/fs/";
  filePath.append(filename);
  return filePath;
}

bool CFile::Open()
{
  mIsOpen = true;
  return mIsOpen;
}

bool CFile::Close()
{
  if (mIsOpen)
  {
    mFile.close();
    mIsOpen = false;
  }
  return true;
}

bool CFile::Exist()
{
  return WiFiStorage.exists(mFilePath.c_str());
}

void CFile::Delete()
{
  mFile.erase();
}

size_t CFile::Size()
{
  return mFile.size();
}

size_t CFile::Read(void* data, size_t size)
{
  if (!Open())
    return 0;

  return mFile.read(data, size);
}

size_t CFile::Write(const void* data, size_t size)
{
  if (!Open())
    return 0;

  return mFile.write(data, size);
}
