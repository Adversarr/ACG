set(CPM_DOWNLOAD_VERSION 0.35.5)

if(CPM_SOURCE_CACHE)
  # Expand relative path. This is important if the provided path contains a tilde (~)
  get_filename_component(CPM_SOURCE_CACHE ${CPM_SOURCE_CACHE} ABSOLUTE)
  set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
elseif(DEFINED ENV{CPM_SOURCE_CACHE})
  set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
else()
  set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
endif()

if(NOT(EXISTS ${CPM_DOWNLOAD_LOCATION}))
  message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
  file(DOWNLOAD
    https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
    ${CPM_DOWNLOAD_LOCATION}
    SHOW_PROGRESS
  )
endif()

include(${CPM_DOWNLOAD_LOCATION})


# Note: CPM usage:
# 
# CPMAddPackage("gh:catchorg/Catch2@2.5.0")
# CPMAddPackage("gh:ericniebler/range-v3#0.12.0")
# CPMAddPackage("gh:jbeder/yaml-cpp#yaml-cpp-0.6.3@0.6.3")
# CPMAddPackage(
#   NAME nlohmann_json
#   VERSION 3.9.1
#   GITHUB_REPOSITORY nlohmann/json
#   OPTIONS
#     "JSON_BuildTests OFF"
# )
# CPMAddPackage(
#   NAME Boost
#   VERSION 1.77.0
#   GITHUB_REPOSITORY "boostorg/boost"
#   GIT_TAG "boost-1.77.0"
# )

# CPMAddPackage(
#   GITHUB_REPOSITORY jarro2783/cxxopts
#   VERSION 2.2.1
#   OPTIONS "CXXOPTS_BUILD_EXAMPLES NO" "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_ENABLE_INSTALL YES"
# )


# CPMAddPackage(
#   NAME benchmark
#   GITHUB_REPOSITORY google/benchmark
#   VERSION 1.5.2
#   OPTIONS "BENCHMARK_ENABLE_TESTING Off"
# )

# if(benchmark_ADDED)
#   # enable c++11 to avoid compilation errors
#   set_target_properties(benchmark PROPERTIES CXX_STANDARD 11)
# endif()

# CPMAddPackage(
#   NAME lua
#   GIT_REPOSITORY https://github.com/lua/lua.git
#   VERSION 5.3.5
#   DOWNLOAD_ONLY YES
# )

# if(lua_ADDED)
#   # lua has no CMake support, so we create our own target
#   FILE(GLOB lua_sources ${lua_SOURCE_DIR}/*.c)
#   list(REMOVE_ITEM lua_sources "${lua_SOURCE_DIR}/lua.c" "${lua_SOURCE_DIR}/luac.c")
#   add_library(lua STATIC ${lua_sources})

#   target_include_directories(lua
#     PUBLIC
#     $<BUILD_INTERFACE:${lua_SOURCE_DIR}>
#   )
# endif()