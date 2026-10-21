#!/usr/bin/bash

cmake -S . -B build_orin \
    -D CMAKE_TOOLCHAIN_FILE=toolchain-orin.cmake \
    -D FOONATHAN_MEMORY_BUILD_EXAMPLES=OFF \
    -D FOONATHAN_MEMORY_BUILD_TESTS=OFF \
    -D FOONATHAN_MEMORY_BUILD_TOOLS=ON \
    -D CMAKE_POSITION_INDEPENDENT_CODE=ON \
    -D CMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build_orin -j4

cmake --install build_orin --prefix ~/thirdparty/Fast-DDS/target_orin
