add_executable(
  demo
  main.cpp
)

target_link_libraries(
  demo
  utilities
  config
  scripting
  cpp_compile_options
  CONAN_PKG::fmt 
)

add_dependencies(
  demo
  pythonengine
  rubyengine
)
