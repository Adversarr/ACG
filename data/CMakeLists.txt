# Dataset Module

# Find all the source files.
file(GLOB_RECURSE ACG_DATA_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/source/**.cpp")
# Declare the library.
find_package(tinyobjloader CONFIG REQUIRED)
add_library(acg_data STATIC
  ${ACG_DATA_SOURCE})
# Setup the include directories.
target_include_directories(acg_data PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
# Dependent only on core
target_link_libraries(acg_data
  PUBLIC
  acg_core
  tinyobjloader::tinyobjloader)
target_compile_definitions(acg_data PRIVATE DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data/")

find_package(doctest CONFIG REQUIRED)
file(GLOB_RECURSE ADATA_TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
add_executable(acg_data_test
${ADATA_TEST_SRC})

target_link_libraries(acg_data_test PRIVATE doctest::doctest acg_data)
include(doctest)
