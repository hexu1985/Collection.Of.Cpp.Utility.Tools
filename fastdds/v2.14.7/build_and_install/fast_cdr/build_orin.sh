#!/usr/bin/bash

cmake -S . -B build_orin \
    -D CMAKE_TOOLCHAIN_FILE=toolchain-orin.cmake \
    -D CMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build_orin -j4

cmake --install build_orin --prefix ~/thirdparty/Fast-DDS/target_orin
#cmake --install build_orin --prefix ~/local/fastcdr-2.2.8
