# Workflow
add_library(workflow OBJECT
  Measure.hpp
  Measure.cpp
  OSArgument.hpp
  OSArgument.cpp
  Runner.hpp
  Runner.cpp
)

target_include_directories(workflow INTERFACE ./)

target_link_libraries(workflow PRIVATE
  cpp_compile_options
  cpp_warning_options
  CONAN_PKG::fmt
)

target_compile_definitions(workflow PRIVATE openstudio_measure_EXPORTS SHARED_OS_LIBS)
if(WIN32)
  #class 'std::optional<std::string>' needs to have dll-interface to be used by clients of class 'openstudio::OSArgument'
  target_compile_options(workflow PRIVATE /wd4251)
endif()
