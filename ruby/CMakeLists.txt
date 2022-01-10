# Ruby

set_source_files_properties(../scripting/OpenStudio.i PROPERTIES CPLUSPLUS ON)

#set(SWIG_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/rubybindings.cxx")
#
#add_custom_command(
#  OUTPUT "${SWIG_OUTPUT}"
#  COMMAND ${CMAKE_COMMAND} -E env SWIG_LIB="${SWIG_LIB}"
#          "${SWIG_EXECUTABLE}"
#          #"-ruby" "-c++" "-fvirtual" "-I${PROJECT_SOURCE_DIR}/src" "-I${PROJECT_BINARY_DIR}/src" "${extra_includes}" "${extra_includes2}" ${RUBY_AUTODOC}
#          "-ruby" "-c++" "-fvirtual" "-I${PROJECT_SOURCE_DIR}"
#          #-module "${MODULE}" -initname "${LOWER_NAME}"
#          #"-I${PROJECT_SOURCE_DIR}/ruby"
#          -o "${SWIG_OUTPUT}"
#          #"${SWIG_DEFINES}" ${SWIG_COMMON} "${PROJECT_SOURCE_DIR}/scripting/OpenStudio.i"
#          ${SWIG_DEFINES}
#          ${SWIG_COMMON}
#          "${PROJECT_SOURCE_DIR}/scripting/OpenStudio.i"
#          #DEPENDS ${this_depends}
#)

#include_directories(${PROJECT_SOURCE_DIR})
#
#add_library(
#  rubybindings OBJECT
#  ${SWIG_OUTPUT}
#)

swig_add_library(
  rubybindings TYPE STATIC
  LANGUAGE ruby 
  OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}
  SOURCES ../scripting/OpenStudio.i
)

target_link_libraries(
  rubybindings
  PRIVATE CONAN_PKG::openstudio_ruby openstudiolib
)

add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
  COMMAND
  ${CMAKE_COMMAND} -E env SWIG_LIB="${SWIG_DIR}" "${SWIG_EXECUTABLE}" "-v"
  "-ruby" -external-runtime
  "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
)

#set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx" PROPERTIES COMPILE_FLAGS "-DRUBY_EMBEDDED -Wno-unused-variable" )

add_library(
  rubyengine
  SHARED
  engine/RubyEngine.hpp
  engine/RubyEngine.cpp
  #${SWIG_OUTPUT}
  ${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx
)

target_include_directories(
  rubyengine 
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)

# For some unknown reason timer_create and other timer symbols are,
# not included by the linker, even when explicitly linking the rt library
# where these symbols are located.
# Without "whole_archives" there is a runtime (not link time) error
add_library(whole_archives INTERFACE)

set_target_properties(whole_archives PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(whole_archives INTERFACE
  -Wl,--whole-archive
  rt
  -Wl,--no-whole-archive
)

target_link_libraries(
  rubyengine
  PRIVATE rubybindings CONAN_PKG::openstudio_ruby cpp_compile_options whole_archives
)

