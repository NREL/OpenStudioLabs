cmake_minimum_required(VERSION 3.17.5)

project(OpenStudioLabs VERSION 0.0.1)

# Compiler options and other cmake configuration
include(config/config.cmake)
# Dependencies
include(conan/conan.cmake)

option(USE_RUBY_ENGINE "Use RubyEngine" ON)
option(USE_PYTHON_ENGINE "Use PythonEngine" ON)

# TODO: temp on Apple the std isn't set to 20
# lib/workflow/./Measure.hpp:22:24: warning: defaulted function definitions are a C++11 extension [-Wc++11-extensions]
#   virtual ~Measure() = default;
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Subprojects
add_subdirectory(scripting)
add_subdirectory(embedded)
add_subdirectory(lib)
if(USE_RUBY_ENGINE)
  add_subdirectory(ruby)
endif()
if(USE_PYTHON_ENGINE)
  add_subdirectory(python)
endif()
add_subdirectory(cli)
