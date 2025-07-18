#!/usr/bin/bash

cmake -S. -Bbuild_orin -DCMAKE_TOOLCHAIN_FILE=toolchain-orin.cmake
cmake --build build_orin
cmake --install build_orin --prefix ~/orin/install
