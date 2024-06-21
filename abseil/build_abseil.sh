cmake -S . -B build -DCMAKE_PREFIX_PATH=~/local -DCMAKE_INSTALL_PREFIX=~/local -DABSL_ENABLE_INSTALL=ON -DABSL_USE_EXTERNAL_GOOGLETEST=ON -DABSL_FIND_GOOGLETEST=ON
cmake --build build
cmake --build build --target install
