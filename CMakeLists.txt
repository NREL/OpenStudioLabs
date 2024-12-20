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


find_package(Ruby 3.2.2 EXACT REQUIRED)
message("Ruby_LIBRARIES=${Ruby_LIBRARIES}")

find_package(Python 3.12.2 EXACT REQUIRED COMPONENTS Interpreter Development OPTIONAL_COMPONENTS NumPy)

# Conan
find_package(minizip)
find_package(ZLIB)
find_package(fmt)
find_package(SWIG 4.1.1 EXACT REQUIRED CONFIG COMPONENTS python ruby csharp)
message("SWIG_EXECUTABLE=${SWIG_EXECUTABLE}")
message("SWIG_DIR=${SWIG_DIR}")
include(UseSWIG)

# Subprojects
add_subdirectory(scripting)
# add_subdirectory(embedded)
add_subdirectory(lib)
add_subdirectory(python)
add_subdirectory(ruby)
add_subdirectory(cli)
