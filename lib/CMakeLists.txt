# openstudio

add_subdirectory(model)
add_subdirectory(workflow)

add_library(openstudiolib SHARED
  empty.cpp
)

target_include_directories(openstudiolib INTERFACE util)

target_link_libraries(openstudiolib
  model workflow ${CMAKE_DL_LIBS}
)
