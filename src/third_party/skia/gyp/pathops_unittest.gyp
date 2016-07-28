# GYP file to build pathops unit tests.
{
  'includes': [
    'apptype_console.gypi',
  ],
  'targets': [
    {
      'target_name': 'pathops_unittest',
      'type': 'executable',
      'includes': [
        'pathops_unittest.gypi',
      ],
      'dependencies': [
        'flags.gyp:flags_common',
        'tools.gyp:crash_handler',
      ],
      'sources': [
        '../tests/PathOpsAngleIdeas.cpp',
        '../tests/PathOpsBattles.cpp',
        '../tests/PathOpsCubicLineIntersectionIdeas.cpp',
        '../tests/PathOpsDebug.cpp',
        '../tests/PathOpsOpLoopThreadedTest.cpp',
        '../tests/skia_test.cpp',
        '../src/utils/SkTaskGroup.cpp',
      ],
      'conditions': [
        [ 'skia_android_framework == 1', {
          'libraries': [
            '-lskia',
          ],
          'libraries!': [
            '-lz',
            '-llog',
          ],
        }],
        [ 'skia_gpu == 1', {
          'include_dirs': [
            '../src/gpu',
          ],
        }],
      ],
    },
  ],
}
