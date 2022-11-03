local spv_home = '$(buildir)/shader/outputs/'


add_requires("glslang", {configs = {binaryonly = true}})
add_requires('glm')
add_requires('glfw')
add_requires('coost')
add_requires("cmake::Vulkan", { alias = 'vulkansdk', system = true })

target('acg_visualizer')
  set_kind('static')
  add_includedirs('include', { public = true })
  add_files('src/**.cpp')
  add_packages('vulkansdk', 'glm', 'glfw')
  add_deps('acg_core', 'acg_utils', 'imgui-glfw-vulkan')
  add_defines('SPV_HOME=\"' ..  spv_home .. '\"')
target_end()

target('shaders')
  set_kind('object')
  add_rules('utils.glsl2spv', {outputdir = spv_home, bin2c=true})
  add_files('shader/*.vert', 'shader/*.frag')
  add_packages('glslang')
target_end()

target('acg_visualizer_exec')
  set_default(true)
  add_files('standalone/*.cpp')
  add_deps('acg_visualizer')
target_end()

