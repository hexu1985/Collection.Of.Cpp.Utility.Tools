cmake -S. -Bbuild_x86
cmake --build build_x86 --parallel 4
cmake --install build_x86 --prefix ~/thirdparty/Fast-DDS/target_x86
