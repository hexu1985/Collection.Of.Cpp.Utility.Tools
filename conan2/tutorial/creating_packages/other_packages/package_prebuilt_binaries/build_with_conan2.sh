
#conan new cmake_lib -d name=hello -d version=0.1

#conan install . -s build_type=Release

cmake -S. -Bbuild/Release -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=build/Release/generators/conan_toolchain.cmake
cmake --build build/Release

conan export-pkg . -s build_type=Release
