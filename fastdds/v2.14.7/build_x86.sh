#!/usr/bin/bash

current_file_dir=$( cd $(dirname ${BASH_SOURCE[0]}) && pwd )
cd ${current_file_dir}
project_root_dir=$( pwd )

echo ${project_root_dir}
cmake -S. -Bbuild_x86 -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DSECURITY=OFF \
    -DFASTDDS_ENFORCE_LOG_INFO=ON \
    -DCOMPILE_EXAMPLES=ON \
    -DPERFORMANCE_TESTS=ON \
    -DCMAKE_INSTALL_PREFIX=${project_root_dir}/target_x86 \
    -DAsio_INCLUDE_DIR=${project_root_dir}/target_x86/include
cmake --build build_x86 -j4

cmake --install build_x86 --prefix ~/local/fastdds-2.14.7
#-DNO_TLS=ON \
