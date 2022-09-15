project(Core)
include(../cmake/CPM.cmake)
file(GLOB srcs src/*.cpp)
file(GLOB headers include/*.h)



include(../cmake/eigen.cmake)

add_library(AGL_Core ${srcs} ${headers})

target_include_directories(AGL_Core PUBLIC include)

target_link_libraries(AGL_Core 
  PUBLIC 
    Eigen3::Eigen)

if (PROJECT_IS_TOP_LEVEL OR AGL_Core_TEST_ENABLE)
  # enable test
  message(STATUS "Enable Test for Core.")
  add_subdirectory(test)
endif()