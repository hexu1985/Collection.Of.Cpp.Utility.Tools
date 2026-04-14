#!/usr/bin/bash

cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=~/fast_dds/install \
    -DAsio_INCLUDE_DIR=~/fast_dds/asio-1.18.1/include \
    -DCOMPILE_EXAMPLES=ON \
    -DFASTDDS_ENFORCE_LOG_INFO=ON
cmake --build build -j4
#cmake --install build --prefix ~/local/fast_dds
