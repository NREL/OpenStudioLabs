# Python

# **Only** find static libs
set(Python_USE_STATIC_LIBS FALSE)

find_package(
  Python
  COMPONENTS Interpreter Development
  REQUIRED
)

set_source_files_properties(
  ../scripting/OpenStudio.i
  PROPERTIES
  CPLUSPLUS ON
)

swig_add_library(
  pythonbindings TYPE STATIC
  LANGUAGE python 
  OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}
  SOURCES ../scripting/OpenStudio.i
)

set_target_properties(pythonbindings
  PROPERTIES
  SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON
)

target_include_directories(
  pythonbindings 
  INTERFACE ${CMAKE_CURRENT_BINARY_DIR}
  PRIVATE ${Python_INCLUDE_DIRS}
)

target_link_libraries(
  pythonbindings
  PRIVATE openstudiolib
)

set (EXTENSION_PY "${CMAKE_CURRENT_BINARY_DIR}/openstudio.py")
foreach( _FILE ${EXTENSION_PY} )
  file(RELATIVE_PATH LOCATION ${CMAKE_CURRENT_BINARY_DIR} ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS "python/${LOCATION}")
endforeach()

include(../embedded/EmbedFiles.cmake)
embed_files("${FILES}" "${EMBEDDED_PATHS}" EMBEDDED_OUTPUT)
set_source_files_properties(${EMBED_SOURCE_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group(embedded_files FILES ${EMBEDDED_OUTPUT})

add_library(
  pythonengine
  SHARED
  engine/PythonEngine.hpp
  engine/PythonEngine.cpp
  ${EMBEDDED_OUTPUT}
  "${CMAKE_CURRENT_BINARY_DIR}/embedded_files.cxx"
  "${CMAKE_CURRENT_BINARY_DIR}/embedded_files.hxx"
)

target_include_directories(pythonengine INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/engine)
target_include_directories(pythonengine PRIVATE ${PROJECT_BINARY_DIR})

target_link_libraries(
  pythonengine
  PRIVATE pythonbindings openstudiolib Python::Python cpp_compile_options CONAN_PKG::zlib
)
