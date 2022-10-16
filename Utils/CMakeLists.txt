project(acg_utils LANGUAGES CXX)
set(MODULE_NAME agl_utils)
include(../cmake/CPM.cmake)
include(../cmake/co.cmake)
include(../cmake/spdlog.cmake)

file(GLOB_RECURSE srcs src/*.cpp)
file(GLOB_RECURSE headers include/*.h include/*.hpp)

add_library(${PROJECT_NAME} ${srcs} ${headers})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC coost::co spdlog::spdlog)

if(PROJECT_IS_TOP_LEVEL OR ${PROJECT_NAME}_TEST_ENABLE)
  # enable test.
  message(STATUS "Enable Test for ${PROJECT_NAME}")
  add_subdirectory(test)
endif()
