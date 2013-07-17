#
# Copyright (C) 2013 Google Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

{
  'includes': [
    '../WebKit/chromium/WinPrecompile.gypi',
    '../core/core.gypi',
    '../modules/modules.gypi',
    'bindings.gypi',
  ],

  'variables': {
    'deprecated_perl_idl_files': [
      '<@(deprecated_perl_core_idl_files)',
      '<@(deprecated_perl_modules_idl_files)',
      '<@(deprecated_perl_svg_idl_files)',
    ],
    'python_idl_files': [
      '<@(python_core_idl_files)',
      '<@(python_modules_idl_files)',
      '<@(python_svg_idl_files)',
    ],
    'perl_and_python_idl_files': [
        '<@(deprecated_perl_idl_files)',
        '<@(python_idl_files)',
    ],

    'bindings_output_dir': '<(SHARED_INTERMEDIATE_DIR)/webkit/bindings',
    'generated_global_constructors_idl_files': [
         '<(SHARED_INTERMEDIATE_DIR)/WindowConstructors.idl',
         '<(SHARED_INTERMEDIATE_DIR)/WorkerGlobalScopeConstructors.idl',
         '<(SHARED_INTERMEDIATE_DIR)/SharedWorkerGlobalScopeConstructors.idl',
         '<(SHARED_INTERMEDIATE_DIR)/DedicatedWorkerGlobalScopeConstructors.idl',
    ],

    'conditions': [
      ['OS=="win" and buildtype=="Official"', {
        # On windows official release builds, we try to preserve symbol space.
        'derived_sources_aggregate_files': [
          '<(bindings_output_dir)/V8DerivedSourcesAll.cpp',
        ],
      },{
        'derived_sources_aggregate_files': [
          '<(bindings_output_dir)/V8DerivedSources01.cpp',
          '<(bindings_output_dir)/V8DerivedSources02.cpp',
          '<(bindings_output_dir)/V8DerivedSources03.cpp',
          '<(bindings_output_dir)/V8DerivedSources04.cpp',
          '<(bindings_output_dir)/V8DerivedSources05.cpp',
          '<(bindings_output_dir)/V8DerivedSources06.cpp',
          '<(bindings_output_dir)/V8DerivedSources07.cpp',
          '<(bindings_output_dir)/V8DerivedSources08.cpp',
          '<(bindings_output_dir)/V8DerivedSources09.cpp',
          '<(bindings_output_dir)/V8DerivedSources10.cpp',
          '<(bindings_output_dir)/V8DerivedSources11.cpp',
          '<(bindings_output_dir)/V8DerivedSources12.cpp',
          '<(bindings_output_dir)/V8DerivedSources13.cpp',
          '<(bindings_output_dir)/V8DerivedSources14.cpp',
          '<(bindings_output_dir)/V8DerivedSources15.cpp',
          '<(bindings_output_dir)/V8DerivedSources16.cpp',
          '<(bindings_output_dir)/V8DerivedSources17.cpp',
          '<(bindings_output_dir)/V8DerivedSources18.cpp',
          '<(bindings_output_dir)/V8DerivedSources19.cpp',
        ],
      }],
      # The bindings generator can not write generated files if they are identical
      # to the already existing file – that way they don't need to be recompiled.
      # However, a reverse dependency having a newer timestamp than a
      # generated binding can confuse some build systems, so only use this on
      # ninja which explicitly supports this use case (gyp turns all actions into
      # ninja restat rules).
      ['"<(GENERATOR)"=="ninja"', {
        'write_file_only_if_changed': '--write-file-only-if-changed 1',
      },{
        'write_file_only_if_changed': '--write-file-only-if-changed 0',
      }],
    ],
  },

  'target_defaults': {
    'variables': {
      'optimize': 'max',
    },
  },

  'targets': [{
    'target_name': 'interface_dependencies',
    'type': 'none',
    'actions': [{
      'action_name': 'compute_interface_dependencies',
      'variables': {
        # Write sources into a file, so that the action command line won't
        # exceed OS limits.
        'idl_files_list': '<|(idl_files_list.tmp <@(perl_and_python_idl_files))',
      },
      'inputs': [
        'scripts/compute_dependencies.py',
        '<(idl_files_list)',
        '<!@(cat <(idl_files_list))',
       ],
       'outputs': [
         '<(SHARED_INTERMEDIATE_DIR)/InterfaceDependencies.txt',
         '<@(generated_global_constructors_idl_files)',
         '<(SHARED_INTERMEDIATE_DIR)/EventNames.in',
       ],
       'msvs_cygwin_shell': 0,
       'action': [
         'python',
         'scripts/compute_dependencies.py',
         '--idl-files-list',
         '<(idl_files_list)',
         '--interface-dependencies-file',
         '<(SHARED_INTERMEDIATE_DIR)/InterfaceDependencies.txt',
         '--window-constructors-file',
         '<(SHARED_INTERMEDIATE_DIR)/WindowConstructors.idl',
         '--workerglobalscope-constructors-file',
         '<(SHARED_INTERMEDIATE_DIR)/WorkerGlobalScopeConstructors.idl',
         '--sharedworkerglobalscope-constructors-file',
         '<(SHARED_INTERMEDIATE_DIR)/SharedWorkerGlobalScopeConstructors.idl',
         '--dedicatedworkerglobalscope-constructors-file',
         '<(SHARED_INTERMEDIATE_DIR)/DedicatedWorkerGlobalScopeConstructors.idl',
         '--event-names-file',
         '<(SHARED_INTERMEDIATE_DIR)/EventNames.in',
         '<@(write_file_only_if_changed)',
       ],
       'message': 'Resolving partial interfaces dependencies in all IDL files',
      }]
    },
    {
      'target_name': 'deprecated_perl_bindings_sources',
      'type': 'none',
      # The 'binding' rule generates .h files, so mark as hard_dependency, per:
      # https://code.google.com/p/gyp/wiki/InputFormatReference#Linking_Dependencies
      'hard_dependency': 1,
      'dependencies': [
        'interface_dependencies',
        '../core/core_derived_sources.gyp:generate_test_support_idls',
      ],
      'sources': [
        '<@(deprecated_perl_idl_files)',
        '<@(deprecated_perl_webcore_test_support_idl_files)',
      ],
      'rules': [{
        'rule_name': 'deprecated_perl_binding',
        'extension': 'idl',
        'msvs_external_rule': 1,
        'inputs': [
          'scripts/deprecated_generate_bindings.pl',
          'scripts/deprecated_code_generator_v8.pm',
          'scripts/deprecated_idl_parser.pm',
          'scripts/deprecated_idl_serializer.pm',
          '../core/scripts/preprocessor.pm',
          'scripts/IDLAttributes.txt',
          # FIXME: If the dependency structure changes, we rebuild all files,
          # since we're not computing dependencies file-by-file in the build.
          '<(SHARED_INTERMEDIATE_DIR)/InterfaceDependencies.txt',
          # FIXME: Similarly, if any partial interface changes, rebuild
          # everything, since every IDL potentially depends on them, because
          # we're not computing dependencies file-by-file.
          #
          # If a new partial interface is added, need to regyp to update these
          # dependencies, as these are computed statically at gyp runtime.
          '<!@pymod_do_main(list_idl_files_with_partial_interface <@(perl_and_python_idl_files))',
          # Generated IDLs are all partial interfaces, hence everything
          # potentially depends on them.
          '<@(generated_global_constructors_idl_files)',
        ],
        'outputs': [
          '<(bindings_output_dir)/V8<(RULE_INPUT_ROOT).cpp',
          '<(bindings_output_dir)/V8<(RULE_INPUT_ROOT).h',
        ],
        'variables': {
          # IDL include paths. The generator will search recursively for IDL
          # files under these locations.
          'generator_include_dirs': [
            '--include', '../core',
            '--include', '../modules',
            '--include', '<(SHARED_INTERMEDIATE_DIR)/webkit',
          ],
          # Hook for embedders to specify extra directories to find IDL files.
          'extra_blink_generator_include_dirs%': [],
        },
        'msvs_cygwin_shell': 0,
        # sanitize-win-build-log.sed uses a regex which matches this command
        # line (Perl script + .idl file being processed).
        # Update that regex if command line changes (other than changing flags)
        'action': [
          '<(perl_exe)',
          '-w',
          '-Iscripts',
          '-I../core/scripts',
          '-I../../../JSON/out/lib/perl5',
          'scripts/deprecated_generate_bindings.pl',
          '--outputDir',
          '<(bindings_output_dir)',
          '--idlAttributesFile',
          'scripts/IDLAttributes.txt',
          '<@(generator_include_dirs)',
          '<@(extra_blink_generator_include_dirs)',
          '--interfaceDependenciesFile',
          '<(SHARED_INTERMEDIATE_DIR)/InterfaceDependencies.txt',
          '--additionalIdlFiles',
          '<(deprecated_perl_webcore_test_support_idl_files)',
          '<@(preprocessor)',
          '<@(write_file_only_if_changed)',
          '<(RULE_INPUT_PATH)',
        ],
        'message': 'Generating binding from <(RULE_INPUT_PATH)',
      }],
    },
    {
      'target_name': 'python_bindings_sources',
      'type': 'none',
      # The 'binding' rule generates .h files, so mark as hard_dependency, per:
      # https://code.google.com/p/gyp/wiki/InputFormatReference#Linking_Dependencies
      'hard_dependency': 1,
      'dependencies': [
        'interface_dependencies',
        '../core/core_derived_sources.gyp:generate_test_support_idls',
      ],
      'sources': [
        '<@(python_idl_files)',
        '<@(python_webcore_test_support_idl_files)',
      ],
      'rules': [{
        'rule_name': 'python_binding',
        'extension': 'idl',
        'msvs_external_rule': 1,
        'inputs': [
          'scripts/idl_compiler.py',
          'scripts/idl_reader.py',
          'scripts/interface_dependency_resolver.py',
          'scripts/code_generator_v8.py',
          'scripts/IDLAttributes.txt',
          # FIXME: If the dependency structure changes, we rebuild all files,
          # since we're not computing dependencies file-by-file in the build.
          '<(SHARED_INTERMEDIATE_DIR)/InterfaceDependencies.txt',
          # FIXME: Similarly, if any partial interface changes, rebuild
          # everything, since every IDL potentially depends on them, because
          # we're not computing dependencies file-by-file.
          '<!@pymod_do_main(list_idl_files_with_partial_interface <@(perl_and_python_idl_files))',
          # Generated IDLs are all partial interfaces, hence everything
          # potentially depends on them.
          '<@(generated_global_constructors_idl_files)',
        ],
        'outputs': [
          '<(bindings_output_dir)/V8<(RULE_INPUT_ROOT).cpp',
          '<(bindings_output_dir)/V8<(RULE_INPUT_ROOT).h',
        ],
        'variables': {
          # IDL include paths. The generator will search recursively for IDL
          # files under these locations.
          'generator_include_dirs': [
            '--include', '../core',
            '--include', '../modules',
            '--include', '<(SHARED_INTERMEDIATE_DIR)/webkit',
          ],
          # Hook for embedders to specify extra directories to find IDL files.
          'extra_blink_generator_include_dirs%': [],
        },
        'msvs_cygwin_shell': 0,
        # sanitize-win-build-log.sed uses a regex which matches this command
        # line (Python script + .idl file being processed).
        # Update that regex if command line changes (other than changing flags)
        'action': [
          'python',
          'scripts/idl_compiler.py',
          '--output-dir',
          '<(bindings_output_dir)',
          '--idl-attributes-file',
          'scripts/IDLAttributes.txt',
          '<@(generator_include_dirs)',
          '<@(extra_blink_generator_include_dirs)',
          '--interface-dependencies-file',
          '<(SHARED_INTERMEDIATE_DIR)/InterfaceDependencies.txt',
          '--additional-idl-files',
          '<(python_webcore_test_support_idl_files)',
          '<@(write_file_only_if_changed)',
          '<(RULE_INPUT_PATH)',
        ],
        'message': 'Generating binding from <(RULE_INPUT_PATH)',
      }],
    },
    {
      'target_name': 'bindings_derived_sources',
      'type': 'none',
      'dependencies': [
        'interface_dependencies',
        'deprecated_perl_bindings_sources',
        'python_bindings_sources',
      ],
      'actions': [{
        'action_name': 'derived_sources_all_in_one',
        'inputs': [
          '../core/scripts/action_derivedsourcesallinone.py',
          '<(SHARED_INTERMEDIATE_DIR)/InterfaceDependencies.txt',
        ],
        'outputs': [
          '<@(derived_sources_aggregate_files)',
        ],
        'action': [
          'python',
          '../core/scripts/action_derivedsourcesallinone.py',
          '<(SHARED_INTERMEDIATE_DIR)/InterfaceDependencies.txt',
          '--',
          '<@(derived_sources_aggregate_files)',
        ],
        'message': 'Generating bindings derived sources',
      }],
    },
  ],
}
