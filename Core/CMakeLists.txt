project(acg_core LANGUAGES CXX)
include(../cmake/CPM.cmake)
file(GLOB srcs src/*.cpp)
file(GLOB headers include/*.h)



include(../cmake/eigen.cmake)

add_library(acg_core ${srcs} ${headers})

target_include_directories(acg_core PUBLIC include)

target_link_libraries(acg_core 
  PUBLIC 
    Eigen3::Eigen
    acg_utils)

if (PROJECT_IS_TOP_LEVEL OR acg_core_TEST_ENABLE)
  # enable test
  message(STATUS "Enable Test for Core.")
  add_subdirectory(test)
endif()
