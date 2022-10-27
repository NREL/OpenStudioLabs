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

if (UNIX)
  set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/OpenStudioRUBY_wrap.cxx" PROPERTIES COMPILE_FLAGS "-Wno-unused-variable" )
endif()

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

target_link_libraries(
  rubyengine
  PRIVATE
    CONAN_PKG::openstudio_ruby
    rubybindings
    cpp_compile_options
)

target_compile_definitions(rubyengine PRIVATE openstudio_scriptengine_EXPORTS SHARED_OS_LIBS)

if(WIN32)
  target_link_libraries(rubyengine
    PRIVATE
    wsock32 ws2_32 Dbghelp Shlwapi Iphlpapi
  )

  target_link_options(rubyengine PRIVATE $<$<CONFIG:Debug>:/NODEFAULTLIB:MSVCRT>)
endif()

if (APPLE)
  find_library(COREFOUNDATION_LIBRARY CoreFoundation)
  target_link_libraries(rubyengine PRIVATE ${COREFOUNDATION_LIBRARY})
  set_target_properties(rubyengine PROPERTIES SUFFIX ".so")
endif()
