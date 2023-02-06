cmake_minimum_required(VERSION 3.17.5)

project(OpenStudioLabs VERSION 0.0.1)

find_package(SWIG)

# Compiler options and other cmake configuration
include(config/config.cmake)
# Dependencies
include(conan/conan.cmake)

# Subprojects
add_subdirectory(scripting)
add_subdirectory(embedded)
add_subdirectory(lib)
add_subdirectory(python)
add_subdirectory(ruby)
add_subdirectory(cli)
add_subdirectory(demo)
