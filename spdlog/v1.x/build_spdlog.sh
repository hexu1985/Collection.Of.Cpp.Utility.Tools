cmake -S. -Bbuild -DSPDLOG_BUILD_PIC=ON -DSPDLOG_BUILD_SHARED=ON
#    -DCMAKE_INSTALL_PREFIX=~/local/spdlog 
cmake --build build -j 2 #--parallel 4 #--target install
cmake --install build --prefix ~/local/spdlog
