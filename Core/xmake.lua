add_requires('eigen')
add_requires('coost')
add_requires('spdlog', {configs={fmt_external = true}})
add_requires('fmt')


target("acg_core")
  set_kind('static')
  add_includedirs('include', {public = true})
  add_files('src/**.cpp')
  add_packages('eigen', 'coost', 'spdlog', 'fmt', {public = true})
target_end()

target('acg_core_test')
  add_files('test/**.cpp')
  add_packages('coost')
  add_deps('acg_core')
target_end()
