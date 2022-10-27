# cli

add_executable(
  openstudio
  main.cpp
)

message(AUTHOR_WARNING "USE_RUBY_ENGINE=${USE_RUBY_ENGINE}, USE_PYTHON_ENGINE=${USE_PYTHON_ENGINE}")

if(USE_RUBY_ENGINE)
  target_compile_definitions(openstudio PUBLIC USE_RUBY_ENGINE=1)
endif()

if(USE_PYTHON_ENGINE)
  target_compile_definitions(openstudio PUBLIC USE_PYTHON_ENGINE=1)
endif()

target_link_libraries(
  openstudio
  PRIVATE
    openstudiolib
    config
    scripting
    cpp_compile_options
    CONAN_PKG::fmt
    workflow
)
