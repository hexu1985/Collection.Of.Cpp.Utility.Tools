#!/usr/bin/bash

cmake -S. -Bbuild_x86 -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build_x86 -j4

cmake --install build_x86 --prefix ~/thirdparty/Fast-DDS/target_x86
