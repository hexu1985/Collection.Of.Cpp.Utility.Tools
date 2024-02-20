#!/usr/bin/env bash

cd `dirname ${BASH_SOURCE[0]}`
cmake -H. -Bbuild
VERBOSE=1 cmake --build build
echo "target dir is build"
