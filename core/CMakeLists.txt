find_package(Eigen3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(cxxopts CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Taskflow CONFIG REQUIRED)

file(GLOB_RECURSE ACG_CORE_SOURCE CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/source/**.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/**.hpp")

add_library(acg_core STATIC
  ${ACG_CORE_SOURCE})

target_include_directories(acg_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(acg_core
  PUBLIC
  cxxopts::cxxopts
  Eigen3::Eigen
  nlohmann_json::nlohmann_json
  spdlog::spdlog
  spdlog::spdlog_header_only
  Taskflow::Taskflow)

find_package(doctest CONFIG REQUIRED)
include(doctest)
file(GLOB_RECURSE ACORE_TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
add_executable(acg_core_test
  ${ACORE_TEST_SRC})

target_link_libraries(acg_core_test PRIVATE doctest::doctest acg_core)

# doctest_discover_tests(
# acg_core_test
# )
acg_set_warn_levels(acg_core)