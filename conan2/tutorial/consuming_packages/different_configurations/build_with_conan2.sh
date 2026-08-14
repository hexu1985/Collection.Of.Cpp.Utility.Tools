
#conan install . --output-folder=build --build=missing
#conan install . --output-folder=build --build=missing --settings=build_type=Debug
conan install . --output-folder=build --build=missing --options=zlib/1.3.1:shared=True --settings=build_type=Debug
source build/conanbuild.sh

cmake -S. -Bbuild -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# source build/deactivate_conanbuild.sh
