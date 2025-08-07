#!/usr/bin/bash

current_dir=$( cd $(dirname ${BASH_SOURCE[0]}) && pwd )
src_dir=${current_dir}
build_dir=${current_dir}/build

cmake -S ${src_dir} -B ${build_dir} -DCMAKE_INSTALL_PREFIX=~/workspace/dvpn-lib-sys-zkbs/target_x86
#cmake -S ${src_dir} -B ${build_dir} \
#    -DCMAKE_BUILD_TYPE=Debug \
#    -Dfastcdr_DIR=${HOME}/fast_dds/install/lib/cmake/fastcdr \
#    -Dfastrtps_DIR=${HOME}/fast_dds/install/share/fastrtps/cmake \
#    -Dfoonathan_memory_DIR=${HOME}/fast_dds/install/lib/foonathan_memory/cmake \
#    -Dtinyxml2_DIR=${HOME}/fast_dds/install/lib/cmake/tinyxml2
cmake --build ${build_dir} -j 4

