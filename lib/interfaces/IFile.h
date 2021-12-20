#pragma once

#include <cstdlib>

namespace Interfaces
{

class IFile
{
  virtual bool Exist() = 0;
  virtual void Delete() = 0;

  virtual size_t Size();
  virtual size_t Read(void* data, size_t size) = 0;
  virtual size_t Write(const void* data, size_t size) = 0;
};

} // namespace Interfaces