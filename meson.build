project(
  'jd', 'c',
  license: 'GPL-3.0-or-later',
  version: '0.0.0-git'
)

# Try to get version string from git or use project version if that fails.
git_describe = run_command('git', 'describe', '--tags', check: false)
git_describe_out = git_describe.stdout().strip()

version = meson.project_version()

if git_describe_out != ''
  version = git_describe_out.substring(1)
endif

# Define a variable for configuration data.
conf_data = configuration_data()

# Set up configuration data.
conf_data.set('version', version)
conf_data.set('tag', 'v' + version)

# Set up an include list.
incdir = include_directories('include')

# Set up a source list.
src = files(
  'src/jd-config.c',
  'src/jd-cd.c',
  'src/jd-ls.c',
  'src/main.c'
)

# Execute the include subdir.
subdir('include')

# Execute the doc subdir.
subdir('doc')

jd = executable(
  'jd', src,
  include_directories: incdir,
  install: true,
)
