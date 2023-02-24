set_group('examples')
add_requires('taskflow')
target('mass-spring')
  add_files('mass_spring/**.cpp')
  add_deps('acg_gui', 'acg_core')
  add_packages('taskflow')
target_end()

target('ggui')
  add_files('ggui/**.cpp')
  add_deps('acg_gui', 'acg_core')
target_end()

target('ggui2')
  add_files('ggui2/**.cpp')
  add_deps('acg_gui', 'acg_core')
target_end()

add_requires('benchmark', 'autodiff')
target('bench_sad')
  add_files('bench_sad/*.cpp')
  add_deps('acg_core')
  add_packages('benchmark', 'autodiff')
target_end()

target('bench_math_utils')
  add_files('bench_math_utils/*.cpp')
  add_deps('acg_core')
  add_packages('benchmark')
target_end()

target('mpm-explicit')
  add_files('mpm-explicit/**.cpp')
  add_deps('acg_gui')
target_end()

target('fem-explicit')
  add_files('fem-explicit/**.cpp')
  add_deps("acg_gui")
target_end()

target('fem-implicit') 
  add_files('fem-implicit/**.cpp')
  add_deps('acg_gui')
target_end()


