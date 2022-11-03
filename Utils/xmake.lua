add_requires('coost')
add_requires('spdlog')

target('acg_utils')
  set_languages('c++17')
  add_includedirs('include', {public = true})
  add_files('src/*.cpp')
  set_kind('static')
  add_packages('coost', 'spdlog', {public = true})
target_end()

target("acg_utils_test")
  set_languages('c++17')
  set_kind('binary')
  add_files('test/**.cpp')
  add_deps('acg_utils')
  -- add_packages('coost')
target_end()
