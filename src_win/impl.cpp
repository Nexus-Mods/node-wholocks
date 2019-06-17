#include "impl.h"

#include "tinyformat.h"
#include "scopeguard.h"

#include <vector>
#include <string>

#include <windows.h>
#include <restartmanager.h>
#pragma comment(lib, "Rstrtmgr.lib")


class WILException : public std::exception {
public:
  explicit WILException(const std::string &message)
    : m_Message(message)
  {
  }

  virtual ~WILException() noexcept {}

  virtual const char* what() const throw () {
    return m_Message.c_str();
  }


private:
  std::string m_Message;
};


std::vector<Process> WhoIsLocking(const std::wstring &path)
{
  std::vector<Process> result;

  DWORD handle;
  WCHAR sessionKey[CCH_RM_SESSION_KEY + 1] = { 0 };
  DWORD res = RmStartSession(&handle, 0, sessionKey);
  if (res != ERROR_SUCCESS) {
    throw WILException(tfm::format("RM: Failed to start session (Errorcode %lu)", res));
  }

  ON_BLOCK_EXIT([handle]() {
    RmEndSession(handle);
  });

  LPCWSTR pathP = path.c_str();
  res = RmRegisterResources(handle, 1, &pathP, 0, nullptr, 0, nullptr);
  if (res != ERROR_SUCCESS) {
    throw WILException(tfm::format("RM: Failed to register resource (Errorcode %lu)", res));
  }

  UINT numInfoNeeded = 0;
  UINT numInfo = 2;
  std::vector<RM_PROCESS_INFO> info(numInfo);
  DWORD reason;

  res = RmGetList(handle, &numInfoNeeded, &numInfo, &info[0], &reason);
  // it's possible, although very unlikely that between two calls of getlist the number of
  // processes changes. Hence repeat until we actually get everything
  while (res == ERROR_MORE_DATA) {
    numInfo = numInfoNeeded;
    info.resize(numInfo);
    res = RmGetList(handle, &numInfoNeeded, &numInfo, &info[0], &reason);
  }

  if (res != ERROR_SUCCESS) {
    if (res == ERROR_ACCESS_DENIED) {
      // the restart manager api supports only files and reports access denied for directories.
      // We could see if any file in the directory is locked but that would be slow and still
      // wouldn't be too reliable as applications with their cwd on the directory lock it as well
      numInfo = 0;
    }
    else {
      throw WILException(tfm::format("RM: Failed to get list of processes (Errorcode %lu)", res));
    }
  }


  for (UINT i = 0; i < numInfo; ++i) {
    Process proc = { info[i].Process.dwProcessId, info[i].strAppName };
    result.push_back(proc);
  }

  return result;
}
