#!/usr/bin/env bash

cd `dirname ${BASH_SOURCE[0]}`
cmake -S. -Bbuild -D absl_DIR=${HOME}/local/abseil-cpp/lib/cmake/absl
cmake --build build --parallel 8
echo "target dir is build"
