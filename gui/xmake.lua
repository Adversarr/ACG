local spv_home = '/shader/outputs/'
add_requires("glslang", {configs = {binaryonly = true}})
add_requires('glm', {system=false})
add_requires('glfw')
add_requires('coost')
add_requires('vulkansdk', {system=true})
add_requires('imgui', {configs = {glfw_vulkan = true}})

target('acg_gui')
  set_kind('static')
  add_includedirs('include', { public = true })
  add_files('src/**.cpp')
  add_packages('glslang', 'vulkansdk', 'glm', 'glfw', 'imgui', {public=true})
  add_deps('acg_core')
  -- set_pcxxheader("include/acg_vis/avk.hpp")

  add_rules('utils.glsl2spv', {outputdir = "$(buildir)" .. spv_home, bin2c=false})
  add_files('shader/*.vert', 'shader/*.frag')
  
  on_load(function(target)
    import('core.project.config')
    local build_dir = config.get("buildir")
    build_dir = path.absolute(build_dir)
    target:add("defines", 'SPV_HOME="' .. string.gsub(build_dir, '\\', '/') .. spv_home .. '"')
  end)
  
  after_build(function (tg)
    if os.isdir("$(projectdir)/data/shaders/") then
      os.mkdir("$(projectdir)/data/shaders/")
    end
    os.rm("$(projectdir)/data/shaders/*.spv")
    os.cp("$(buildir)" .. spv_home .. "/*.spv", "$(projectdir)/data/shaders/")
  end)
target_end()

