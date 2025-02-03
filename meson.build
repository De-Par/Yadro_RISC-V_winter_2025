project(
    'YadroHack',
    ['c', 'cpp'],
    version: '0.1',
)

# dependencies 
opencl_dep = dependency('OpenCL')
libpng_dep = dependency('libpng')

# useful info
cpp = meson.get_compiler('cpp')
c = meson.get_compiler('c')
project_dir = meson.current_source_dir()
build_dir = meson.current_build_dir()
host_system = host_machine.system()

subdir('src')

#windows
if host_system == 'windows'
    message('Building for windows...\n')
# macos
elif host_system == 'darwin'
    message('Building for macos...\n')
# linux
elif host_system == 'linux'
    message('Building for linux...\n')
endif

# make executable
executable(
    'vecsum', 
    vec_sum_files,
    dependencies: [
       opencl_dep, 
    ],
    cpp_args: [
        '-std=c++14',
        #'-Ofast',
    ],
    install: true
)              