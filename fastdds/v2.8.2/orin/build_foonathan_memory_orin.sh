#!/usr/bin/bash

cmake -S. -Bbuild_orin -DCMAKE_TOOLCHAIN_FILE=toolchain-orin.cmake -DFOONATHAN_MEMORY_BUILD_TESTS=OFF
cmake --build build_orin
cmake --install build_orin --prefix ~/orin/install
