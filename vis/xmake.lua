local spv_home = '$(buildir)/shader/outputs/'



add_requires("glslang", {configs = {binaryonly = true}})
add_requires('glm', {system=false})
add_requires('glfw')
add_requires('coost')
add_requires('vulkansdk', {system=true})
add_requires('imgui', {configs = {glfw_vulkan = true}})


target('acg_visualizer')
  set_kind('static')
  set_languages('cxx17')
  add_includedirs('include', { public = true })
  add_files('src/**.cpp')
  add_packages('vulkansdk', 'glm', 'glfw', 'imgui', {public=true})
  add_deps('acg_core')
  add_defines('SPV_HOME=\"' ..  spv_home .. '\"')
target_end()

target('shaders')
  set_kind('object')
  add_rules('utils.glsl2spv', {outputdir = spv_home, bin2c=true})
  add_files('shader/*.vert', 'shader/*.frag')
  add_packages('glslang')
target_end()

