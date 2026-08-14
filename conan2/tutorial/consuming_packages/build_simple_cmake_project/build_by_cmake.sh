
#conan install . --output-folder=build --build=missing

cmake -S. -Bbuild -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build
