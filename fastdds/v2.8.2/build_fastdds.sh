#!/usr/bin/bash

cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=~/fast_dds/install
cmake --build build
mkdir -p ~/local
cmake --install build --prefix ~/local/fast_dds
