# Physics Module:
# 1. FEM/MassSpring/... Energy, Force, Hessian of Energy,
# 2. Data structure definitions.


# Find all the source files.
file(GLOB_RECURSE ACG_PHYSICS_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/source/**.cpp")
# Declare the library.
add_library(acg_physics STATIC
  ${ACG_PHYSICS_SOURCE})
# Setup the include directories.
target_include_directories(acg_physics PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
# Dependent only on core
target_link_libraries(acg_physics
  PUBLIC
  acg_core)

find_package(doctest CONFIG REQUIRED)
include(doctest)
file(GLOB_RECURSE APHYSICS_TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
add_executable(acg_physics_test
  ${APHYSICS_TEST_SRC})

target_link_libraries(acg_physics_test PRIVATE doctest::doctest acg_physics)

doctest_discover_tests(
  acg_physics_test
)