current_dir=$( cd $(dirname ${BASH_SOURCE[0]}) && pwd )
src_dir=${current_dir}
build_dir=${current_dir}/build

cmake -S ${src_dir} -B ${build_dir} -DYAML_CPP_DIR=${HOME}/local/yaml-cpp -DCMAKE_BUILD_TYPE=Debug
cmake --build ${build_dir} -j 4
