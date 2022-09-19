project(agl_visualizer)

include(../cmake/CPM.cmake)

file(GLOB_RECURSE srcs src/*.cpp)
file(GLOB_RECURSE headers include/*.h include/*.hpp)

# Dependency: Vulkan
find_package(Vulkan REQUIRED)

# Dependency: GLM
CPMAddPackage(NAME glm
  GITHUB_REPOSITORY g-truc/glm
  GIT_TAG "0.9.9.8")

# Dependency: GLFW3
find_package(glfw3 REQUIRED)

# Dependency:
CPMAddPackage(NAME imgui
  GITHUB_REPOSITORY ocornut/imgui
  GIT_TAG v1.88)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SOURCE
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_demo.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp)

add_library(${PROJECT_NAME} ${srcs} ${headers} ${IMGUI_SOURCE})

target_include_directories(${PROJECT_NAME} PUBLIC
  "include"
  ${IMGUI_DIR} ${IMGUI_DIR}/backends)

target_link_libraries(${PROJECT_NAME} PUBLIC
  acg_utils
  acg_core
  Vulkan::Vulkan
  glm::glm
  glfw)

set_target_properties(${PROJECT_NAME} PROPERTIES LANGUAGE CXX LINKER_LANGUAGE CXX)

file(GLOB exec_src standalone/*.cpp standalone/*.cxx)
add_executable(${PROJECT_NAME}_exec ${exec_src})
target_include_directories(${PROJECT_NAME}_exec PUBLIC standalone)
target_link_libraries(${PROJECT_NAME}_exec PUBLIC ${PROJECT_NAME})