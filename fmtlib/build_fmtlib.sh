cmake -H. -Bbuild -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_INSTALL_PREFIX=~/local/fmtlib 
cmake --build build --parallel 4 --target install
#cmake --install build
