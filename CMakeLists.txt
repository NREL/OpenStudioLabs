project(Demo)
set(CMAKE_CXX_STANDARD 20)

include(CheckIncludeFile)
include(CheckIncludeFileCXX)
check_include_file("unistd.h" HAVE_UNISTD_H)

include(GenerateExportHeader)

add_library(b SHARED
  b.hpp
  b.cpp
)
generate_export_header(b)
target_include_directories(b PUBLIC ${PROJECT_BINARY_DIR})

add_library(a SHARED
  a.hpp
  a.cpp
)
generate_export_header(a)
target_include_directories(a PUBLIC ${PROJECT_BINARY_DIR})

target_link_libraries(a PRIVATE b)

add_executable(demo
  main.cpp
)

target_link_libraries(demo PRIVATE a b ${CMAKE_DL_LIBS})

if(HAVE_UNISTD_H)
  target_compile_definitions(demo PUBLIC "-DHAVE_UNISTD_H")
endif()
