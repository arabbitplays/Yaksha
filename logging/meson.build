project('logging_lib',
        ['cpp', 'c'],
        default_options : [
            'warning_level=3',
            'cpp_std=c++20'
        ],
)

incdirs = [
    include_directories('include'),
]

run_command('bash', './generate_meson_files.sh', 'src', 'src', check: true)

src = []
subdir('src')

lib = library(
    'logging_lib',
    sources: src,
    include_directories: incdirs,
    install : true,
)

install_subdir(
    'include/logging',
    install_dir: get_option('includedir')
)

pkg = import('pkgconfig')

pkg.generate(
    lib,
    name: 'logging_lib',
    description: 'A simple cpp logging library',
    version: meson.project_version()
)

logging_dep = declare_dependency(
    link_with: lib,
    include_directories: incdirs,
)

meson.override_dependency('logging_lib', logging_dep)
