#!/usr/bin/env bash

cd `dirname ${BASH_SOURCE[0]}`
cmake -H. -Bbuild
cmake --build build --parallel 4
echo "target dir is build"
