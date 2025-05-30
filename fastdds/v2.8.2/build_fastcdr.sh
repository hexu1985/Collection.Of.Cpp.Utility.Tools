#!/usr/bin/bash

cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=Debug
cmake --build build
cmake --install build --prefix ~/workspace/install
