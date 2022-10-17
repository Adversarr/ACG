function(target_include_shader_directory TARGET_NAME DIRECTORY)
  file(GLOB shader_source CONFIGURE_DEPENDS
    "${DIRECTORY}/*.vert"
    "${DIRECTORY}/*.frag"
    "${DIRECTORY}/*.comp"
    "${DIRECTORY}/*.geom"
    "${DIRECTORY}/*.tesc"
    "${DIRECTORY}/*.tese"
    "${DIRECTORY}/*.mesh"
    "${DIRECTORY}/*.task"
    "${DIRECTORY}/*.rgen"
    "${DIRECTORY}/*.rchit"
    "${DIRECTORY}/*.rmiss"
    "${DIRECTORY}/*.rcall")
  message(NOTICE "${shader_source}")

  foreach(shader ${shader_source})
    get_filename_component(file_name ${shader} NAME)
    set(output_spv_name "${DIRECTORY}/${file_name}.spv")
    add_custom_command(
      OUTPUT ${output_spv_name}
      COMMAND ${Vulkan_GLSLC_EXECUTABLE}
      ARGS ${shader} -o ${output_spv_name} --target-env=vulkan
      COMMENT "Build Shader ${output_spv_name}"
      DEPENDS ${shader}
      VERBATIM
    )
    list(APPEND custom_target ${output_spv_name})
  endforeach()

  add_custom_target(${TARGET_NAME}_shaders DEPENDS ${custom_target})
  add_dependencies(${TARGET_NAME} ${TARGET_NAME}_shaders)
  target_sources(${TARGET_NAME} PUBLIC ${custom_target})
  target_compile_definitions(${TARGET_NAME} PRIVATE -DSPV_HOME="${DIRECTORY}")
endfunction()