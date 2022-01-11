cmake_minimum_required(VERSION 3.17.5)

project(test_swig VERSION 0.0.1)

add_library(cpp_warning_options INTERFACE)
add_library(cpp_compile_options INTERFACE)

target_compile_features(cpp_compile_options INTERFACE cxx_std_17)

include(CheckIncludeFile)
check_include_file("unistd.h" HAVE_UNISTD_H)
if(HAVE_UNISTD_H)
  target_compile_definitions(cpp_compile_options INTERFACE "-DHAVE_UNISTD_H")
endif()

if(MSVC)
  target_compile_options(cpp_warning_options INTERFACE /W4 /Werror)
else()
  #  target_compile_options(
  #    cpp_warning_options INTERFACE -Wall -Wextra -Wconversion -Werror -Wno-unused-variable)
#  target_compile_options(cpp_compile_options
#                         INTERFACE -fsanitize=undefined,address)
#  target_link_options(cpp_compile_options INTERFACE
#                      -fsanitize=undefined,address)
endif()

# Do not enable compiler specific extensions, for eg on GCC use -std=c++1z
# (=c++17) and not -std=gnu++17
set(CMAKE_CXX_EXTENSIONS OFF)

# Use ccache is available, has to be before "project()"
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  # Support Unix Makefiles and Ninja
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

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

# ##############################################################################
#                          C M A K E    C O N T R O L                          #
# ##############################################################################

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

# Search first in the binary dir, where conan will install finders, then search
# for modules in the root dir to override cmake ones
list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}"
     "${PROJECT_SOURCE_DIR}/CMake")

# Add to include path

# Project source directory
include_directories("${PROJECT_SOURCE_DIR}")
###############################################################################

###############################################################################
# Compiler and system specific options
if(UNIX)

  add_compile_definitions(WITHRUBY)
  add_compile_definitions(RUBY_EMBEDDED)

  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fPIC -fno-strict-aliasing")

  # all warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -fno-strict-aliasing -Winvalid-pch -Wnon-virtual-dtor")
  # Treat all warnings as errors
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
  if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual -ftemplate-depth=1024")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual")
    # DLM: had to add this due to issues with boost optional and gcc, may be resolved if we move to std::optional
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-maybe-uninitialized")
  endif()

  # Note: CMAKE_CXX_STANDARD (with no extensions) already sets -std=c++17 (or std=c++1z).
  if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  endif()
endif()

if(MINGW)
  # all warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  # effective c++
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++")
  # treat warnings as errors
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

  # wchar not supported for MinGW
  #add_definitions(-DBOOST_LOG_USE_CHAR)
  #add_definitions(-DBOOST_LOG_NO_COMPILER_TLS)
endif()

if(MSVC)
  # warning level 3
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")

  # warning level 4 - DLM: we should shoot for this
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")

  # all warnings - DLM: probably too high to ever use
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall")

  # create compiler processes on all effective processors
  # /MP is useless for ninja and will cause problems. BUILD_WITH_MULTIPLE_PROCESSES shouldn't have been set if ninja but let's be safe
  if(BUILD_WITH_MULTIPLE_PROCESSES AND NOT NINJA)
    add_definitions(/MP)
  endif()

  # Enable warning on thread un-safe static member initialization - DLM: we have been bit by this many times
  # TODO: JM 2019-09-19: Disabling for now
  # Using the righg flag (/w14460 and not the previously mispelled /w44640) will make compilation fail with many warnings treated as error,
  # eg: IddRegex.cpp all scoped `const static std::string result` inside functions will issue that and make it fatal
  # add_definitions(/w14640)

  # ignore decorated name length exceeded
  add_definitions(/wd4503)

  # ignore needs to have dll-interface to be used by clients of class
  # we will not rely on exporting templates instead all libraries must
  # be linked against the same runtime
  add_definitions(/wd4251)

  # ignore base class not dll-exported error
  add_definitions(/wd4275)

  add_definitions(/bigobj)

  if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bit
    # ignore conversion from size_t to int for now
    add_definitions(/wd4267)
  else()
    # ignore conversion from 'type1' to 'type2', possible loss of data
    add_definitions(/wd4244)
  endif()

  # treat warnings as errors
  # DLM: only do this for our code, added in later
  # add_definitions(/WX)

  # ignore warnings about the stl being insecure
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)

  # ignore c++17 deprecation warnings generated by boost
  add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
  add_definitions(-D_SILENCE_FPOS_SEEKPOS_DEPRECATION_WARNING)

endif()

##############################################################################

#######################################################################
#                              C O N A N                              #
#######################################################################

set(CMAKE_CONAN_EXPECTED_HASH 170c3250029af321395135a3952a9045)
set(CMAKE_CONAN_VERSION "v0.16.1")

if(EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  file(MD5 "${CMAKE_BINARY_DIR}/conan.cmake" CMAKE_CONAN_HASH)
endif()
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake" OR NOT "${CMAKE_CONAN_HASH}" MATCHES "${CMAKE_CONAN_EXPECTED_HASH}")
  # Put it in CMAKE_BINARY_DIR so we don't end up with two when building OpenStudioApplication
  message(STATUS "Downloading conan.cmake ${CMAKE_CONAN_VERSION} from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/${CMAKE_CONAN_VERSION}/conan.cmake"
     "${CMAKE_BINARY_DIR}/conan.cmake")
else()
  message(STATUS "Using existing conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_check(VERSION 1.28.0 REQUIRED)

message(STATUS "openstudio: RUNNING CONAN")

# Add NREL remote and place it first in line, since we vendored dependencies to NREL's repo, they will be picked first
# TJC 2021-04-27 bintray.com is decommissioned as of 2021-05-01. See commercialbuildings as replacement below.
conan_add_remote(NAME nrel INDEX 0
   URL https://conan.commercialbuildings.dev/artifactory/api/conan/openstudio)

conan_add_remote(NAME bincrafters
  URL https://bincrafters.jfrog.io/artifactory/api/conan/public-conan)

# Enable revisions in conan: check if they are already enabled, if not do it and warn user
execute_process(
  COMMAND ${CONAN_CMD} config get general.revisions_enabled
  OUTPUT_VARIABLE CONAN_REV_STATUS
  ERROR_VARIABLE CONAN_REV_STATUS
  OUTPUT_STRIP_TRAILING_WHITESPACE)

message(STATUS "Conan: config get general.revisions_enabled=${CONAN_REV_STATUS}")
if (NOT "${CONAN_REV_STATUS}" STREQUAL "True")
  message(AUTHOR_WARNING "Conan: Force enabling revisions (conan config set general.revisions_enabled=True)")
  execute_process(COMMAND ${CONAN_CMD} config set general.revisions_enabled=True)
endif()

list(APPEND CONAN_OPTIONS "zlib:minizip=True")
list(APPEND CONAN_BUILD "missing")

conan_cmake_run(REQUIRES
  "openstudio_ruby/2.7.2"
  "zlib/1.2.11#0df31fd24179543f5720ec7beb2a88d7"
  "swig/4.0.2#bfafb16cd2bea6af3b8003163abcbd09"
  "fmt/6.1.2"
  ${CONAN_GTEST}
  BASIC_SETUP CMAKE_TARGETS NO_OUTPUT_DIRS
  OPTIONS ${CONAN_OPTIONS}
  BUILD ${CONAN_BUILD}
  # Passes `-u, --update`    to conan install: Check updates exist from upstream remotes
  # That and build=outdated should ensure we track the right
  # Now that we pin dependencies, there is no point looking upstream really, so we'll save valuable configuration time by not doing it
  UPDATE
)

message("CONAN_LIBS=${CONAN_LIBS}")

macro(FindValue ValueName)
  set(LocalVar "")
  set(LocalVar $<$<CONFIG:Debug>:${${ValueName}_DEBUG}>$<$<CONFIG:Release>:${${ValueName}_RELEASE}>$<$<CONFIG:RelWithDebInfo>:${${ValueName}_RELWITHDEBINFO}>$<$<CONFIG:MinSizeRel>:${${ValueName}_MINSIZEREL}>
  )
#  list(JOIN LocalVar "" LocalVar)
  string(STRIP ${LocalVar} LocalVar)
  set(CURRENT_${ValueName} $<IF:$<BOOL:${LocalVar}>,${LocalVar},${${ValueName}}>)
  # For debug purposes
  # message(STATUS "Found '${ValueName}' as '${CURRENT_${ValueName}}'")
endmacro()

FindValue(CONAN_SWIG_ROOT)
set(SWIG_LIB "${CURRENT_CONAN_SWIG_ROOT}/bin/swiglib")

# A macro to find a conan related value especially when using multi-config builds (MSVC)
# But it also works with single-config builds
macro(FindValue ValueName)
  set(LocalVar "")
  set(LocalVar $<$<CONFIG:Debug>:${${ValueName}_DEBUG}>$<$<CONFIG:Release>:${${ValueName}_RELEASE}>$<$<CONFIG:RelWithDebInfo>:${$ValueName}_RELWITHDEBINFO}>$<$<CONFIG:MinSizeRel>:${${ValueName}_MINSIZEREL}>
  )
#  list(JOIN LocalVar "" LocalVar)
  string(STRIP ${LocalVar} LocalVar)
  set(CURRENT_${ValueName} $<IF:$<BOOL:${LocalVar}>,${LocalVar},${${ValueName}}>)
  # For debug purposes
  # message(STATUS "Found '${ValueName}' as '${CURRENT_${ValueName}}'")
endmacro()

if(BUILD_RUBY_BINDINGS)
  get_target_property(RUBY_INCLUDE_DIRS CONAN_PKG::openstudio_ruby INTERFACE_INCLUDE_DIRECTORIES)
endif()


# ##############################################################################
# CONFIGURE PATHS            #
# ##############################################################################

configure_file(config.hxx.in config.hxx)

# Search first in the binary dir, where conan will install finders, then
# search for modules in the root dir to override cmake ones
# Start with ROOT, then PROJECT_BINARY_DIR
list(APPEND CMAKE_MODULE_PATH "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/CMake")
include_directories("${CMAKE_CURRENT_BINARY_DIR}")


# ##############################################################################
# SWIG            #
# ##############################################################################
find_package(SWIG REQUIRED COMPONENTS python ruby csharp)
include(${SWIG_USE_FILE})

add_subdirectory(embedded)
add_subdirectory(lib)
add_subdirectory(python)
add_subdirectory(ruby)
add_subdirectory(cli)

