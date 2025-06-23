#!/usr/bin/bash

cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_PREFIX=~/fast_dds/install -DCOMPILE_EXAMPLES=ON \
    -DEPROSIMA_BUILD_TESTS=ON -DGTest_ROOT=~/fast_dds/install
cmake --build build -j4
#cmake --install build
#cmake --install build --prefix ~/local/fast_dds
