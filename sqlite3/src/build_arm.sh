#!/usr/bin/bash

cmake -S. -Bbuild_arm -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake
#-DCMAKE_INSTALL_PREFIX=~/workspace/dvpn-lib-fastdds/target_arm
cmake --build build_arm

#cmake --install build_arm --prefix ~/workspace/dvpn-lib-fastdds/target_arm
#cmake --install build_arm --prefix ~/git/fast-dds-v2.8.2/target_arm
