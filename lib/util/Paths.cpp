#include "Paths.hpp"
#if defined _WIN32
#include <windows.h>
#else
#include <stdio.h>
#include <dlfcn.h>
#endif

namespace openstudio {

  // define the function GetCurrentModule so we can get its address
  #if defined _WIN32
  HMODULE GetCurrentModule()
  {
      HMODULE hModule = NULL;
      // hModule is NULL if GetModuleHandleEx fails.
      GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS
                          | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                          (LPCTSTR)GetCurrentModule, &hModule);
      return hModule;
  }
  #else
  bool GetCurrentModule()
  {
    return true;
  }
  #endif

  std::filesystem::path getCurrentModulePath()
  {
    std::filesystem::path path;
    #if defined _WIN32
      TCHAR szPath[MAX_PATH];
      if( GetModuleFileName( GetCurrentModule(), szPath, MAX_PATH ) ) {
        path = std::filesystem::path(szPath);
      }
    #else
      Dl_info info;
      if (dladdr("GetCurrentModule", &info)) {
        path = std::filesystem::path(info.dli_fname);
      }
    #endif

    return path;
  }

  std::filesystem::path getCurrentModuleDir()
  {
    return getCurrentModulePath().parent_path();
  }

}
