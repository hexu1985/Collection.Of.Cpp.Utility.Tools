#!/usr/bin/env bash

cd `dirname ${BASH_SOURCE[0]}`
cmake -H. -Bbuild -Dgflags_DIR=~/local/gflags/lib/cmake/gflags
cmake --build build --parallel 8
echo "target dir is build"
