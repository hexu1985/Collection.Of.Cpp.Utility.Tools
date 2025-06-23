cmake -H. -Bbuild 
cmake --build build --parallel 4
cmake --install build --prefix ~/fast_dds/install
