#pragma once

#include <string>

namespace Console
{

void Init();
bool WaitForConnection(double timeout);

void Write(const std::string& message);
void WriteLine(const std::string& message);

} // namespace Console