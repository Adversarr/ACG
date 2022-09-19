set(MODULE_NAME agl_visualizer)

include(../cmake/CPM.cmake)

file(GLOB_RECURSE srcs src/*.cpp)
file(GLOB_RECURSE headers include/*.h include/*.hpp)

add_library(${MODULE_NAME} ${srcs} ${headers})

find_package(Vulkan REQUIRED)

target_include_directories(${MODULE_NAME} PUBLIC include)
target_link_libraries(${MODULE_NAME} PUBLIC acg_utils acg_core Vulkan::Vulkan)

set_target_properties(${MODULE_NAME}
  PROPERTIES
  LANGUAGE CXX
  LINKER_LANGUAGE CXX)
