set_config('plat', os.host())

set_project('ACG')

set_languages('cxx17')

set_warnings('all') -- -Wall

add_rules("mode.debug", "mode.release")

set_optimize('faster')


add_moduledirs('xmake')



includes(
  'core',
  'vis'
)

includes('examples')
