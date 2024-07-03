cmake -H. -Bbuild -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE \
    -DCMAKE_INSTALL_PREFIX=~/local/fmtlib 
cmake --build build --target fmt
cmake --install build
