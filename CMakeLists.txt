cmake_minimum_required(VERSION 3.16)

project(acg
  DESCRIPTION "Adversarr's CG Toolbox"
  LANGUAGES CXX)


# Remove this block after bumping CMake to v3.21.0
# PROJECT_IS_TOP_LEVEL is defined then by default
if(CMAKE_VERSION VERSION_LESS 3.21.0)
  if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(PROJECT_IS_TOP_LEVEL TRUE)
  else()
    set(PROJECT_IS_TOP_LEVEL FALSE)
  endif()
endif()

if(PROJECT_IS_TOP_LEVEL)
  set(CMAKE_CXX_STANDARD 17 CACHE STRING "Default C++ standard")
  set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "Require C++ standard")
  set(CMAKE_CXX_EXTENSIONS OFF CACHE BOOL "Allow C++ extensions")
endif()

# guard against in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

# guard against bad build-type strings
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

include(CheckCXXCompilerFlag)
include(GNUInstallDirs)
include(CMakeDependentOption)

include(cmake/utils.cmake)

# Further optimize for SIMD.
if (NOT MSVC)
  acg_add_cxx_compiler_flag("-march=native")
endif()

add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(physics)
add_subdirectory(data)
add_subdirectory(examples)