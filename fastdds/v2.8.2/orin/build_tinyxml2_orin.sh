#!/usr/bin/bash

cmake -S. -Bbuild_orin -DCMAKE_TOOLCHAIN_FILE=toolchain-orin.cmake -DCMAKE_POSITION_INDEPENDENT_CODE=ON
cmake --build build_orin
cmake --install build_orin --prefix ~/orin/install
