#! /bin/sh

export LD_LIBRARY_PATH=$(pwd)/build${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}
mkdir -p reports/valgrind
valgrind --log-file=reports/valgrind/tests-all.log --leak-check=full build/tests/all
exit $?
