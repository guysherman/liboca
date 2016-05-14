#! /bin/sh

export LD_LIBRARY_PATH=$(pwd)/build${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}
echo ""
echo "----------------------------------------------------------------------------------"
echo "Running tests on their own so that crashes crash"
echo ""
build/tests/all

echo ""
echo "----------------------------------------------------------------------------------"
echo "Running tests under valgrind to make sure everything is clean up in there"
echo ""
mkdir -p reports/valgrind
valgrind --log-file=reports/valgrind/tests-all.log --xml=yes --xml-file=reports/valgrind/tests-all.xml --leak-check=full --leak-check-heuristics=none build/tests/all
util/inspect_reports.py reports/cppcheck || exit $?
util/inspect_reports.py reports/valgrind || exit $?
