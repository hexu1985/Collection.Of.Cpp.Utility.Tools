#!/bin/bash

current_dir=$( cd $(dirname ${BASH_SOURCE[0]}) && pwd )
src_dir=${current_dir}
build_dir=${current_dir}/build_orin

cmake -S ${src_dir} -B ${build_dir} -Dtcmalloc_installed_dir="$HOME/local/gperftools_orin" \
    -DCMAKE_TOOLCHAIN_FILE=toolchain-orin.cmake
cmake --build ${build_dir} -j 4

