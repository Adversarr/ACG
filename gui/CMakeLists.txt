find_package(Vulkan REQUIRED COMPONENTS glslc)
find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

# ==============================================================================
# COMPILE SHADERS
set(SHADER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADER_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders)

function(compile_shader_dir TNAME SHADER_SOURCE_DIR SHADER_BINARY_DIR)
  file(GLOB SHADERS
    ${SHADER_SOURCE_DIR}/*.vert
    ${SHADER_SOURCE_DIR}/*.frag
    ${SHADER_SOURCE_DIR}/*.comp
    ${SHADER_SOURCE_DIR}/*.geom
    ${SHADER_SOURCE_DIR}/*.tesc
    ${SHADER_SOURCE_DIR}/*.tese
    ${SHADER_SOURCE_DIR}/*.mesh
    ${SHADER_SOURCE_DIR}/*.task
    ${SHADER_SOURCE_DIR}/*.rgen
    ${SHADER_SOURCE_DIR}/*.rchit
    ${SHADER_SOURCE_DIR}/*.rmiss)

  add_custom_command(
    COMMAND
    ${CMAKE_COMMAND} -E make_directory ${SHADER_BINARY_DIR}
    OUTPUT ${SHADER_BINARY_DIR}
    COMMENT "Creating ${SHADER_BINARY_DIR}"
  )

  foreach(source IN LISTS SHADERS)
    get_filename_component(FILENAME ${source} NAME)
    add_custom_command(
      COMMAND
      ${glslc_executable}
      -o ${SHADER_BINARY_DIR}/${FILENAME}.spv
      ${source}
      OUTPUT ${SHADER_BINARY_DIR}/${FILENAME}.spv
      DEPENDS ${source} ${SHADER_BINARY_DIR}
      COMMENT "Compiling to Spriv: ${FILENAME}"
    )
    list(APPEND SPV_SHADERS ${SHADER_BINARY_DIR}/${FILENAME}.spv)
  endforeach()

  add_custom_target(${TNAME} ALL DEPENDS ${SPV_SHADERS})
endfunction(compile_shader_dir)

set(SPIRV_HOME "${CMAKE_CURRENT_BINARY_DIR}/shader/outputs/")

file(GLOB_RECURSE AGUI_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

add_library(acg_gui STATIC ${AGUI_SOURCE})
target_link_libraries(acg_gui
  PUBLIC
  acg_core
  Vulkan::Vulkan
  imgui::imgui
  glm::glm
  glfw)

target_compile_definitions(acg_gui PRIVATE SPV_HOME="${SPIRV_HOME}")
target_include_directories(acg_gui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

compile_shader_dir(guishader "${CMAKE_CURRENT_SOURCE_DIR}/shader" ${SPIRV_HOME})

add_dependencies(acg_gui guishader)