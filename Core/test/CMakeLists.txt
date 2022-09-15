# add doctest.
CPMAddPackage("gh:onqtam/doctest@2.4.8")
file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(${PROJECT_NAME}_test ${sources})
target_link_libraries(${PROJECT_NAME}_test doctest::doctest AGL_Core)
set_target_properties(${PROJECT_NAME}_test PROPERTIES CXX_STANDARD 17)

# ---- Add GreeterTests ----
enable_testing()

# Note: doctest and similar testing frameworks can automatically configure CMake tests. For other
# testing frameworks add the tests target instead: add_test(NAME ${PROJECT_NAME} COMMAND
# ${PROJECT_NAME})
include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(${PROJECT_NAME}_test)
