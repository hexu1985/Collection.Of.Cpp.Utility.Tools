
conan install . --build=missing -pr:b=default -pr:h=./profiles/raspberry
source build/Release/generators/conanbuild.sh

cmake -S. -Bbuild -DCMAKE_TOOLCHAIN_FILE=Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build

source build/deactivate_conanbuild.sh
