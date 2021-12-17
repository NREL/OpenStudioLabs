# CLItest

add_executable(
  openstudio
  main.cpp
)

target_link_libraries(
  openstudio
  openstudiolib CONAN_PKG::fmt cpp_compile_options
)
