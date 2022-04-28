project('odus', 'cpp',
  version : '0.0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++20'])

sources = ['main.cpp', 'auth/auth.cpp', 'config/config.cpp']
__sources_final = []

foreach p : sources
    __sources_final += ('src/' + p)
endforeach

crypt_dep = dependency('libcrypt')

exe = executable('odus', __sources_final, install : true, include_directories: ['include/'], dependencies: [crypt_dep])
