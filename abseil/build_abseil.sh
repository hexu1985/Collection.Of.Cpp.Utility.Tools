cmake -S . -B build 
cmake --build build -j 4
cmake --install build --prefix ~/local/abseil-cpp
