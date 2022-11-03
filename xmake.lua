set_config('plat', os.host())

set_project('ACG')

set_languages('c++17')

set_warnings('all') -- -Wall
-- set_symbols('debug')

add_rules("mode.debug", "mode.release")

set_optimize('faster')


add_moduledirs('xmake')



includes(
  '3rdparty',
  'Core',
  'Utils',
  'Visualizer'
)