project(
  'jd', 'c',
  license: 'GPL-3.0-or-later',
  version: 'git'
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

# Configure configuration-files.
configure_file(
  input: 'configuration-files/mesonconf.h.in',
  output: 'mesonconf.h',
  configuration: conf_data
)

# Set include directories.
incdir = include_directories(
  '.'
)

# Set up a source list.
src = files(
  'src/main.c'
)

jd = executable(
  'jd', src,
  install: true,
)
