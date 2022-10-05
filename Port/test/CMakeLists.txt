# add doctest.
include(../../cmake/doctest.cmake)

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME}_test ${sources})
target_link_libraries(${PROJECT_NAME}_test doctest::doctest ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}_test PROPERTIES CXX_STANDARD 17)

# ---- Add GreeterTests ----
enable_testing()

# Note: doctest and similar testing frameworks can automatically configure CMake tests. For other
# testing frameworks add the tests target instead: add_test(NAME ${PROJECT_NAME} COMMAND
# ${PROJECT_NAME})
include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(${PROJECT_NAME}_test)

if(MSVC)
  target_compile_options(disabled_but_evaluated PRIVATE /wd4702) # unreachable code
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(${PROJECT_NAME}_test PRIVATE -Wno-global-constructors)
  target_compile_options(${PROJECT_NAME}_test PRIVATE -Wno-unused-variable)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_compile_options(${PROJECT_NAME}_test PRIVATE -Wno-unused-variable)
endif()