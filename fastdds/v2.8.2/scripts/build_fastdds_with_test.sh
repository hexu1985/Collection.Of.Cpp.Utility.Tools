#!/usr/bin/bash

cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DCMAKE_INSTALL_PREFIX=~/fast_dds/install -DCOMPILE_EXAMPLES=ON \
    -DEPROSIMA_BUILD_TESTS=ON -DFASTRTPS_API_TESTS=ON \
    -DGTest_ROOT=~/fast_dds/install -DAsio_INCLUDE_DIR=~/fast_dds/asio-1.18.1/include
cmake --build build -j8
#cmake --install build
#cmake --install build --prefix ~/local/fast_dds
