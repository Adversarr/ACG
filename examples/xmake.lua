set_group('examples')

target('ui_only_world')
  add_files('ui_only_world/**.cpp')
  add_deps('acg_gui')

target('nbody')
  add_files('nbody/**.cpp')
  add_deps('acg_gui', 'acg_core')

target('free-fall')
  add_files('free_fall/**.cpp')
  add_deps('acg_gui', 'acg_core')
target_end()

add_requires('openmp')
target('test_eigen_mp')
  add_files('test_eigen_mp/**.cpp')
  add_deps('acg_core')
  add_packages("openmp")
 
target('mass-spring')
  add_files('mass_spring/**.cpp')
  add_deps('acg_gui', 'acg_core')
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
target('sad_bench')
  add_files('sad_bench/*.cpp')
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

