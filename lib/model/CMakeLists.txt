# Model

add_library(model OBJECT
  Model.hpp
  Model.cpp
  ModelObject.hpp
  ModelObject.cpp
)

target_include_directories(model INTERFACE ./)

target_link_libraries(model PRIVATE
  CONAN_PKG::fmt
  cpp_warning_options
  cpp_compile_options
)
