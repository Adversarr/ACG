target('ui_only_world')
  add_files('ui_only_world/**.cpp')
  add_deps('acg_visualizer')

target('nbody')
  add_files('nbody/**.cpp')
  add_deps('acg_visualizer', 'acg_core')

target('free-fall')
  add_files('free_fall/**.cpp')
  add_deps('acg_visualizer', 'acg_core')
target_end()

add_requires('openmp')
target('test_eigen_mp')
  add_files('test_eigen_mp/**.cpp')
  add_deps('acg_core')
  add_packages("openmp")
 
