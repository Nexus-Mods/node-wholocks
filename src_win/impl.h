#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>
#include <vector>

struct Process {
  DWORD pid;
  std::wstring appName;
};

std::vector<Process> WhoIsLocking(const std::wstring &path);
