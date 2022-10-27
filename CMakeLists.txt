cmake_minimum_required(VERSION 3.17.5)

project(OpenStudioLabs LANGUAGES CXX VERSION 0.0.1)

add_library(cpp_warning_options INTERFACE)
add_library(cpp_compile_options INTERFACE)
target_compile_features(cpp_compile_options INTERFACE cxx_std_20)

# TODO: temp, somehow target_compile_features does not work on Apple, the std isn't set to 20
# lib/workflow/./Measure.hpp:22:24: warning: defaulted function definitions are a C++11 extension [-Wc++11-extensions]
#   virtual ~Measure() = default;
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler options and other cmake configuration
include(config/config.cmake)
# Dependencies
include(conan/conan.cmake)

option(USE_RUBY_ENGINE "Use RubyEngine" ON)
option(USE_PYTHON_ENGINE "Use PythonEngine" ON)

# Subprojects
add_subdirectory(scripting)
add_subdirectory(embedded)
add_subdirectory(lib)
add_subdirectory(python)
add_subdirectory(ruby)
add_subdirectory(cli)
