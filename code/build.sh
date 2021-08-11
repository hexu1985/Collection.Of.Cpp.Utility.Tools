rm -rf ./build
cmake -Bbuild -H. 
#cmake -G"MSYS Makefiles" -Bbuild -H. 
cmake --build build/
#cmake --build build/ -- -j 4
#--target install
