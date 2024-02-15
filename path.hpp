#ifndef DLLDEMO_PATH_HPP
#define DLLDEMO_PATH_HPP

#include <filesystem>

#if __has_include(<unistd.h>)
#include <dlfcn.h>
#include <unistd.h>
#endif

#if __has_include(<windows.h>)
#include <windows.h>
#endif

#if __has_include(<linux/limits.h>)
#include <linux/limits.h>
#endif

#if __has_include(<limits.h>)
#include <climits>
#endif

inline std::filesystem::path EXEPath() {
  auto get_path = []() {
#if _WIN32
    TCHAR szPath[MAX_PATH];
    GetModuleFileName(nullptr, szPath, MAX_PATH);
    return std::filesystem : path(szPath);
#else
    // if /proc/self/exe exists, this should be our best option
    std::array<char, PATH_MAX + 1> buf{};
    if (const auto result =
            ::readlink("/proc/self/exe", buf.data(), buf.size());
        result != -1) {
      return std::filesystem::path(buf.begin(), std::next(buf.begin(), result));
    }

    // otherwise we'll dlopen ourselves and see where "main" exists
    Dl_info info;
    dladdr("main", &info);
    return std::filesystem::path(info.dli_fname);
#endif
  };

  // canonical also makes the path absolute
  static const auto path = std::filesystem::canonical(get_path());
  return path;
}

inline constexpr std::string_view DLLExt() {
#ifdef __APPLE__
  return "dylib";
#elif _WIN32
  return "dll";
#else
  return "so";
#endif
}

inline constexpr std::string_view DLLPrefix() {
#ifdef __APPLE__
  return "lib";
#elif _WIN32
  return "";
#else
  return "lib";
#endif
}

inline std::string DLLFilename(const std::string_view library_name) {
  return std::string(DLLPrefix()) + std::string(library_name) +
         std::string(".") + std::string(DLLExt());
}

#endif // DLLDEMO_PATH_HPP
