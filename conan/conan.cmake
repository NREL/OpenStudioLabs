# Conan

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

conan_add_remote(NAME nrel INDEX 0
   URL https://conan.openstudio.net/artifactory/api/conan/openstudio)

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

conan_cmake_run(REQUIRES
  "openstudio_ruby/2.7.2@nrel/testing#dcea3703b5dfaecfa5f0056d952ea5bd"
  "minizip/1.2.12#0b5296887a2558500d0323c6c94c8d02"
  "zlib/1.2.12#3b9e037ae1c615d045a06c67d88491ae"
  "minizip/1.2.12#0b5296887a2558500d0323c6c94c8d02"
  "fmt/7.0.1#0580b1492b1dddb43b1768e68f25c43c"
  "swig/4.0.2#bfafb16cd2bea6af3b8003163abcbd09"
  BASIC_SETUP CMAKE_TARGETS NO_OUTPUT_DIRS
  BUILD missing
  # Passes `-u, --update`    to conan install: Check updates exist from upstream remotes
  # That and build=outdated should ensure we track the right
  # Now that we pin dependencies, there is no point looking upstream really, so we'll save valuable configuration time by not doing it
  UPDATE
)

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

FindValue(CONAN_SWIG_ROOT)
set(SWIG_LIB "${CURRENT_CONAN_SWIG_ROOT}/bin/swiglib")
find_package(SWIG REQUIRED COMPONENTS python ruby csharp)
include(${SWIG_USE_FILE})
