add_requires('eigen')
add_requires('spdlog', {configs={fmt_external = true}})
add_requires("doctest", {})

add_requires('taskflow')
add_requires('nlohmann_json')

target("acg_core")
  set_kind('static')
  set_group('library')
  add_includedirs('include', {public = true})
  add_files('source/**.cpp')
  add_packages('eigen', 'taskflow','nlohmann_json', 'spdlog', {public = true})
target_end()

target('acg_core_test')
  add_files('test/**.cpp')
  set_group('test')
  add_packages('doctest')
  add_deps('acg_core')
target_end()

