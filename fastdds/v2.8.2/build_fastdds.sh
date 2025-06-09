#!/usr/bin/bash

cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=~/fast_dds/install \
    -DCOMPILE_EXAMPLES=ON \
    -DFASTDDS_ENFORCE_LOG_INFO=ON
cmake --build build -j4
cmake --install build --prefix ~/local/fast_dds
