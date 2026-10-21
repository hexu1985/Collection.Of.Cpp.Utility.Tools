#!/usr/bin/bash

current_file_dir=$( cd $(dirname ${BASH_SOURCE[0]}) && pwd )
cd ${current_file_dir}
project_root_dir=$( pwd )

echo ${project_root_dir}
cmake -S . -B build_orin \
    -D CMAKE_TOOLCHAIN_FILE=toolchain-orin.cmake \
    -D CMAKE_BUILD_TYPE=RelWithDebInfo \
    -D SECURITY=OFF \
    -D FASTDDS_ENFORCE_LOG_INFO=ON \
    -D COMPILE_EXAMPLES=ON \
    -D PERFORMANCE_TESTS=ON \
    -D CMAKE_INSTALL_PREFIX=${project_root_dir}/target_orin \
    -D Asio_INCLUDE_DIR=~/thirdparty/asio-1.18.1/include
cmake --build build_orin -j4

#cmake --install build_orin --prefix ~/local/fastdds-2.14.7
#-DNO_TLS=ON \
