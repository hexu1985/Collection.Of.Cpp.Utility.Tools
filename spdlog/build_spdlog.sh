cmake -H. -Bbuild -DSPDLOG_BUILD_PIC=ON \
    -DCMAKE_INSTALL_PREFIX=~/local/spdlog 
cmake --build build --parallel 4 --target install
#cmake --install build
