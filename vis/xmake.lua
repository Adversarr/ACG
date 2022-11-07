local spv_home = '/shader/outputs/'



add_requires("glslang", {configs = {binaryonly = true}})
add_requires('glm', {system=false})
add_requires('glfw')
add_requires('coost')
add_requires('vulkansdk', {system=true})
add_requires('imgui', {configs = {glfw_vulkan = true}})


target('acg_visualizer')
  set_kind('static')
  add_includedirs('include', { public = true })
  add_files('src/**.cpp')
  add_packages('vulkansdk', 'glm', 'glfw', 'imgui', {public=true})
  add_deps('acg_core')

  on_load(function(target) 
    import('core.project.config')
    local build_dir = config.get("buildir")
    target:add("defines", 'SPV_HOME="' .. string.gsub(build_dir, '\\', '/') .. spv_home .. '"')
  end)
  add_rules('utils.glsl2spv', {outputdir = "$(buildir)" .. spv_home, bin2c=true})
  add_files('shader/*.vert', 'shader/*.frag')
  add_packages('glslang')
target_end()
