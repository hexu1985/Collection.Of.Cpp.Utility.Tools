cmake -S . -B build 
cmake --build build --parallel 4 
cmake --install build --prefix ~/local/fmtlib 
