project(Utilities)
include(../cmake/CPM.cmake)
file(GLOB srcs src/*.cpp)
file(GLOB headers include/*.h)

add_library(AGL_Utils ${srcs} ${headers})

target_include_directories(AGL_Utils PUBLIC include)

if (PROJECT_IS_TOP_LEVEL OR AGL_Utils_TEST_ENABLE)
  # enable test.
  message(STATUS "Enable Test for Utilities")
  add_subdirectory(test)
endif()