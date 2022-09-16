project(Utilities)
include(../cmake/CPM.cmake)
include(../cmake/spdlog.cmake)

file(GLOB_RECURSE srcs src/*.cpp)
file(GLOB_RECURSE headers include/*.h include/*.hpp)

add_library(AGL_Utils ${srcs} ${headers})

target_include_directories(AGL_Utils PUBLIC include)

if (PROJECT_IS_TOP_LEVEL OR AGL_Utils_TEST_ENABLE)
  # enable test.
  message(STATUS "Enable Test for Utilities")
  add_subdirectory(test)
endif()