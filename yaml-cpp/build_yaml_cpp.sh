#!/usr/bin/bash

cmake -S. -Bbuild \
    -DYAML_CPP_BUILD_TESTS=ON \
    -DBUILD_SHARED_LIBS=ON \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build -j4
cmake --install build --prefix ~/local/yaml-cpp
