#pragma once

// C++ generic
#include <string>

// Interfaces
#include "IFile.h"

// Target Specific
#include <WiFiNINA.h>

class CFile : public Interfaces::IFile
{
public:
  CFile(const std::string& filename);
  ~CFile();

  bool Open();
  bool Close();

  bool Exist() override;
  void Delete() override;

  size_t Size() override;
  size_t Read(void* data, size_t size) override;
  size_t Write(const void* data, size_t size) override;

protected:
  std::string mFilePath;
  WiFiStorageFile mFile;
  bool mIsOpen = false;
  std::string GetFilePath(const std::string& filename);
};