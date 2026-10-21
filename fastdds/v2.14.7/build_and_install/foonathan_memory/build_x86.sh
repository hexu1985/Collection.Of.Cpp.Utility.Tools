#!/usr/bin/bash

cmake -S. -Bbuild_x86 \
    -D FOONATHAN_MEMORY_BUILD_EXAMPLES=OFF \
    -D FOONATHAN_MEMORY_BUILD_TESTS=OFF \
    -D FOONATHAN_MEMORY_BUILD_TOOLS=ON \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build_x86 -j4

cmake --install build_x86 --prefix ~/thirdparty/Fast-DDS/target_x86
