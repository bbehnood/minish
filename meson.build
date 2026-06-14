project(
  'minish',
  'c',
  default_options: [
    'c_std=c11',
    'warning_level=3' 
  ]
)

add_project_arguments(
  language: 'c'
)

readline_dep = dependency('readline',)

src = files(
  'src/main.c',
  'src/shell.c',
  'src/token.c',
  'src/lexer.c',
  'src/parser.c',
  'src/exec.c',
  'src/builtins.c',
  'src/utils.c',
)

executable(
  'minish',
  src,
  include_directories: include_directories('include'),
  dependencies: readline_dep,
)
