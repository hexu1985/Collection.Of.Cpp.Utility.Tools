rm -rf ./build
cmake -Bbuild -H. 
cmake --build build/ -- -j 4
#--target install
