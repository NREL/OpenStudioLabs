# Ruby

set_source_files_properties(../scripting/OpenStudio.i PROPERTIES CPLUSPLUS ON)

swig_add_library(
  rubybindings TYPE STATIC
  LANGUAGE ruby 
  OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}
  SOURCES ../scripting/OpenStudio.i
)

set_target_properties(rubybindings
  PROPERTIES
  SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON
)

set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/OpenStudioRUBY_wrap.cxx" PROPERTIES COMPILE_FLAGS "-Wno-unused-variable" )

target_link_libraries(
  rubybindings
  PRIVATE CONAN_PKG::openstudio_ruby openstudiolib cpp_compile_options
)

add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
  COMMAND
  ${CMAKE_COMMAND} -E env SWIG_LIB="${SWIG_DIR}" "${SWIG_EXECUTABLE}" "-v"
  "-ruby" -external-runtime
  "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
)

add_library(
  rubyengine
  SHARED
  engine/RubyEngine.hpp
  engine/RubyEngine.cpp
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
  PRIVATE CONAN_PKG::openstudio_ruby rubybindings cpp_compile_options whole_archives
)

