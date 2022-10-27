# Config

add_library(cpp_warning_options INTERFACE)
add_library(cpp_compile_options INTERFACE)

target_compile_features(cpp_compile_options INTERFACE cxx_std_17)

include(CheckIncludeFile)
check_include_file("unistd.h" HAVE_UNISTD_H)
if(HAVE_UNISTD_H)
  target_compile_definitions(cpp_compile_options INTERFACE "-DHAVE_UNISTD_H")
endif()

if(MSVC)
  target_compile_options(cpp_warning_options INTERFACE /W4 /WX)
else()
  target_compile_options(cpp_warning_options INTERFACE -Wall -Wextra -Wconversion -Werror -Wno-unused-variable)
  target_compile_options(cpp_compile_options INTERFACE -fPIC)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE
      Release
      CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
                                               "MinSizeRel" "RelWithDebInfo")
endif()


# High level project configuration Do we actually want everything to go to
# CMAKE_BINARY_DIR/Products, so that when you build OpenStudioApplication you
# get both OpenStudio (core) and Application in the same place?
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Products")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Products")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Products")

set(LIBRARY_SEARCH_DIRECTORY
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Release"
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Debug")

configure_file(config/config.hxx.in config/config.hxx)
add_library(config INTERFACE)
target_include_directories(config INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/config)

#######################################################################
#             G E N E R A T O R    E X P R E S S I O N S              #
#######################################################################

set(is_msvc_genex "$<CXX_COMPILER_ID:MSVC>")
set(is_gnu_or_clang_genex "$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>")
