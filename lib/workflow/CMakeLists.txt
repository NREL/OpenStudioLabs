# Workflow

add_library(workflow OBJECT
  Measure.cpp
  Measure.hpp
  Runner.hpp
  SpecialRunner.hpp
)

target_include_directories(workflow INTERFACE ./)

target_link_libraries(workflow PRIVATE
  cpp_warning_options
  cpp_compile_options
)
