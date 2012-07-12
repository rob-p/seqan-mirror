#!/usr/bin/env python
"""Execute the tests for %(APP_NAME)s.

The golden test outputs are generated by the script generate_outputs.sh.

You have to give the root paths to the source and the binaries as arguments to
the program.  These are the paths to the directory that contains the 'projects'
directory.

Usage:  run_tests.py SOURCE_ROOT_PATH BINARY_ROOT_PATH
"""
import logging
import os.path
import sys

# Automagically add util/py_lib to PYTHONPATH environment variable.
path = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..',
                                    '..', '..', 'util', 'py_lib'))
sys.path.insert(0, path)

import seqan.app_tests as app_tests

def main(source_base, binary_base):
    """Main entry point of the script."""

    print 'Executing test for %(APP_NAME)s'
    print '========================='
    print
    
    ph = app_tests.TestPathHelper(
        source_base, binary_base,
        '%(LOCATION)s/tests')  # tests dir

    # ============================================================
    # Auto-detect the binary path.
    # ============================================================

    path_to_program = app_tests.autolocateBinary(
      binary_base, '%(LOCATION)s', '%(APP_NAME)s')

    # ============================================================
    # Built TestConf list.
    # ============================================================

    # Build list with TestConf objects, analoguely to how the output
    # was generated in generate_outputs.sh.
    conf_list = []

    # ============================================================
    # First Section.
    # ============================================================

    # App TestConf objects to conf_list, just like this for each
    # test you want to run.
    conf = app_tests.TestConf(
        program=path_to_program,
        redir_stdout=ph.outFile('STDOUT_FILE'),
        args=['ARGS', 'MUST', 'BE', 'STRINGS', str(1), str(99),
              ph.inFile('INPUT_FILE1'),
              ph.inFile('INPUT_FILE2')],
        to_diff=[(ph.inFile('STDOUT_FILE'),
                  ph.outFile('STDOUT_FILE')),
                 (ph.inFile('INPUT_FILE1'),
                  ph.outFile('INPUT_FILE1'))])
    conf_list.append(conf)

    # ============================================================
    # Execute the tests.
    # ============================================================
    failures = 0
    for conf in conf_list:
        res = app_tests.runTest(conf)
        # Output to the user.
        print ' '.join(['%(APP_NAME)s'] + conf.args),
        if res:
             print 'OK'
        else:
            failures += 1
            print 'FAILED'

    # Cleanup.
    ph.deleteTempDir()

    print '=============================='
    print '     total tests: %%d' %% len(conf_list)
    print '    failed tests: %%d' %% failures
    print 'successful tests: %%d' %% (len(conf_list) - failures)
    print '=============================='
    # Compute and return return code.
    return failures != 0


if __name__ == '__main__':
    sys.exit(app_tests.main(main))
