cmake_minimum_required(VERSION 3.17)

project(ACG 
  VERSION 0.0.1
  LANGUAGES C CXX
)


# Typically, I use cxx 17 for better dev.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

 
include(cmake/CPM.cmake)
include(cmake/dependencies.cmake)




option(acg_utils_TEST_ENABLE "Enable test for utilities." ${PROJECT_IS_TOP_LEVEL})
add_subdirectory(Utils)


option(acg_core_TEST_ENABLE "Enable test for core." ${PROJECT_IS_TOP_LEVEL})
add_subdirectory(Core)

add_subdirectory(Visualizer)