#!/usr/bin/bash

cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j8
cmake --install build --prefix ~/local/boost
