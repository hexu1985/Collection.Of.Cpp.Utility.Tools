
#conan install . --output-folder=build --build=missing
#source build/conanbuild.sh

cmake -S. -Bbuild -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build

#source build/deactivate_conanbuild.sh
