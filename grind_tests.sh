#! /bin/sh

export LD_LIBRARY_PATH=$(pwd)/build${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}
mkdir -p reports/valgrind
valgrind --log-file=reports/valgrind/tests-all.log --xml=yes --xml-file=reports/valgrind/tests-all.xml --leak-check=full build/tests/all
util/inspect_reports.py reports/cppcheck || exit $?
util/inspect_reports.py reports/valgrind || exit $?
