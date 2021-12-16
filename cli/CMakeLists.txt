# CLI

add_executable(
  test
  main.cpp
)

target_link_libraries(
  test
  model workflow pythonengine CONAN_PKG::fmt cpp_compile_options
)
