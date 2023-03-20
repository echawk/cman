project('cman',
'c',
default_options: 'c_std=c89',
license: 'GPL3+',
version: '1.0'
)

cc = meson.get_compiler('c')

ncurses_dep = dependency('ncurses')

cman_srcs = ['cman.c', 'enemy_utils.c', 'game_objs.c', 'game_utils.c']

extra_cflags = [
  '-Wall',
  '-pedantic',
  '-D_BSD_SOURCE'
  ]

add_project_arguments(cc.get_supported_arguments(extra_cflags), language : 'c')

executable('cman', cman_srcs, dependencies: ncurses_dep)
