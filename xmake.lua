set_config('plat', os.host())

set_project('ACG')

set_languages('cxx17')

set_warnings('allextra') -- -Wall

add_rules("mode.debug", "mode.release", "mode.releasedbg")

includes(
  '3rdparty',
  'core',
  'port',
  'gui'
)

includes('examples')
