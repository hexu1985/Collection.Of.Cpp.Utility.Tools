#!/usr/bin/bash

current_dir=$( cd $(dirname ${BASH_SOURCE[0]}) && pwd )
src_dir=${current_dir}
build_dir=${current_dir}/build

cmake -S ${src_dir} -B ${build_dir} -DCMAKE_INSTALL_PREFIX=~/workspace/install -DCMAKE_BUILD_TYPE=Debug
#cmake -S ${src_dir} -B ${build_dir} \
#    -DCMAKE_BUILD_TYPE=Debug \
#    -Dfastcdr_DIR=${HOME}/local/fast_cdr/lib/cmake/fastcdr \
#    -Dfastrtps_DIR=${HOME}/local/fast_dds/share/fastrtps/cmake \
#    -Dfoonathan_memory_DIR=${HOME}/local/foonathan_memory/lib/foonathan_memory/cmake \
#    -Dtinyxml2_DIR=${HOME}/local/tinyxml2/lib/cmake/tinyxml2
cmake --build ${build_dir} -j 4

