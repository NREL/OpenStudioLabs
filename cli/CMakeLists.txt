# cli

add_executable(
  openstudio
  main.cpp
)

target_link_libraries(
  openstudio
  openstudiolib 
  config
  scripting
  cpp_compile_options
  CONAN_PKG::fmt 
)
