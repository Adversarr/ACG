-- add_requires("cmake::Vulkan", { alias = 'vulkansdk', system = true })
add_requires('vulkansdk', {system=true})
-- target('imgui-glfw-vulkan')
--   set_kind('static')
--   add_files('imgui/imgui.cpp',
--     'imgui/imgui_draw.cpp',
--     'imgui/imgui_demo.cpp',
--     'imgui/imgui_tables.cpp',
--     'imgui/imgui_widgets.cpp',
--     'imgui/backends/imgui_impl_glfw.cpp',
--     'imgui/backends/imgui_impl_vulkan.cpp')
--   add_includedirs('imgui', { public = true })
--   add_packages('vulkansdk', 'glfw')
--   set_optimize('faster')

add_requires('imgui', {configs = {glfw_vulkan=true}})
