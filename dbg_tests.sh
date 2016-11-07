#! /bin/sh

export LD_LIBRARY_PATH=$(pwd)/build${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}
lldb build/tests/all
exit $?
