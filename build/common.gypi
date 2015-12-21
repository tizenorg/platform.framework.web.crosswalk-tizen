{
  'variables': {
    'build_type%': 'Debug',
    'extension_path%': '<(extension_path)',
    'injected_bundle_path%': '<(injected_bundle_path)',
    'window_system%': '<(window_system)',
  },
  'target_defaults': {
    'variables': {
      'build_type%': '<(build_type)',
    },
    'conditions': [
      ['window_system== "X11"', {
        'defines': ['HAVE_X11'],
      }],
      ['window_system=="Wayland"', {
	'defines': ['HAVE_WAYLAND'],
      }],
      ['build_type== "Debug"', {
        'defines': ['_DEBUG', 'TIZEN_DEBUG_ENABLE', ],
        'cflags': [ '-O0', '-g', ],
      }],
      ['build_type == "Release"', {
        'defines': ['NDEBUG', ],
        'cflags': [
          '-O2',
          # Don't emit the GCC version ident directives, they just end up
          # in the .comment section taking up binary size.
          '-fno-ident',
          # Put data and code in their own sections, so that unused symbols
          # can be removed at link time with --gc-sections.
          '-fdata-sections',
          '-ffunction-sections',
        ],
      }],
    ],
    'includes': [
      'pkg-config.gypi',
      'xwalk_js2c.gypi',
    ],
    'include_dirs': [
      '../',
      '<(SHARED_INTERMEDIATE_DIR)',
    ],
    'defines': [
      'EXTENSION_PATH="<(extension_path)"',
      'INJECTED_BUNDLE_PATH="<(injected_bundle_path)"',
    ],
    'cflags': [
      '-std=c++0x',
      '-fPIC',
      '-fvisibility=hidden',
      '-Wall',
    ],
    'libraries' : [
      '-L./lib',
    ],
  },
}
