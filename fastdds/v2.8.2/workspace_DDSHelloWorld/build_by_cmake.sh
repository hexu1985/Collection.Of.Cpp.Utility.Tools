#!/usr/bin/bash

current_dir=$( cd $(dirname ${BASH_SOURCE[0]}) && pwd )
src_dir=${current_dir}
build_dir=${current_dir}/build

cmake -S ${src_dir} -B ${build_dir} \
    -DCMAKE_BUILD_TYPE=Debug \
    -Dfastcdr_DIR=${HOME}/local/fast_cdr/lib/cmake/fastcdr \
    -Dfastrtps_DIR=${HOME}/local/fast_dds/share/fastrtps/cmake \
    -Dfoonathan_memory_DIR=${HOME}/local/foonathan_memory/lib/foonathan_memory/cmake
cmake --build ${build_dir} -j 4

