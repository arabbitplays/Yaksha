project('DesktopManager',
        ['cpp', 'c'],
        default_options : [
            'warning_level=3',
            'cpp_std=c++20'
        ],
)

incdirs = [
    include_directories('include'),
]

logging_dep = dependency('logging_lib', fallback: ['logging', 'logging_dep'])

project_root = meson.current_source_dir()

py = import('python').find_installation('python3')
run_command(py, 'scripts/generate_meson_files.py', 'src', check: true)

src = []
subdir('src')

exe = executable(
    'DesktopManager',
    sources: src,
    dependencies: [
        logging_dep
    ],
    include_directories: incdirs,
    install : true,
)
