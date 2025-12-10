#!/usr/bin/bash

current_file_dir=$( cd $(dirname ${BASH_SOURCE[0]}) && pwd )
cd ${current_file_dir}
project_root_dir=$( pwd )

echo ${project_root_dir}
cmake -S. -Bbuild -Dgperftools_enable_broken_install_targets=ON
cmake --build build

cmake --install build --prefix ~/local/gperftools
