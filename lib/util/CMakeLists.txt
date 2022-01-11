# Util

add_library(utilities OBJECT
  DynamicLibrary.hpp
  DynamicLibraryPOSIX.hpp
  DynamicLibraryWindows.hpp
  Paths.cpp
  Paths.hpp
)

target_include_directories(utilities INTERFACE ./)

target_link_libraries(utilities PRIVATE
  cpp_warning_options
  cpp_compile_options
)
