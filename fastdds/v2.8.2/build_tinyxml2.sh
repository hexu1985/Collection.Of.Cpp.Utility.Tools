#!/usr/bin/bash

cmake -S. -Bbuild -DCMAKE_POSITION_INDEPENDENT_CODE=ON
cmake --build build
cmake --install build --prefix ~/fast_dds/install
