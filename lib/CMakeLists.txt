# openstudio

add_subdirectory(model)
add_subdirectory(workflow)
add_subdirectory(util)

add_library(openstudiolib STATIC
  empty.cpp
)

target_link_libraries(openstudiolib
  PUBLIC model workflow utilities ${CMAKE_DL_LIBS}
)
