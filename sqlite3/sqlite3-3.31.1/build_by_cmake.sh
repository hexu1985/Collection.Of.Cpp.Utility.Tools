#!/usr/bin/bash

cmake -S. -Bbuild_x86 -DCMAKE_BUILD_TYPE=RelWithDebInfo
#-DCMAKE_INSTALL_PREFIX=~/workspace/dvpn-lib-fastdds/target_x86
cmake --build build_x86

#cmake --install build_x86 --prefix ~/workspace/dvpn-lib-fastdds/target_x86
#cmake --install build_x86 --prefix ~/git/fast-dds-v2.8.2/target_x86
