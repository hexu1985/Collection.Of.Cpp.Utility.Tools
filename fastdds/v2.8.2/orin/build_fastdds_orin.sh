#!/usr/bin/bash

echo ${project_root_dir}
cmake -S. -Bbuild_orin -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=toolchain-orin.cmake \
    -DCMAKE_INSTALL_PREFIX=~/orin/install -DCOMPILE_EXAMPLES=ON \
    -DEPROSIMA_BUILD_TESTS=ON -DFASTRTPS_API_TESTS=ON \
    -DFASTDDS_ENFORCE_LOG_INFO=ON \
    -DGTest_ROOT=~/orin/install -DAsio_INCLUDE_DIR=~/fast_dds/asio-1.18.1/include
#cmake --build build -j8
#cmake --install build
#cmake --install build --prefix ~/local/fast_dds
