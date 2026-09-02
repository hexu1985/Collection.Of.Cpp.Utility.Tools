
# 添加JFrog远程仓库
#conan remote add jfrog https://your-jfrog-server/artifactory/api/conan/conan-local

# 如果需要认证
#conan remote login jfrog your_username -p your_password

source build/Release/generators/conanbuild.sh

conan install . -r artifactory
cmake -S. -Bbuild -DCMAKE_TOOLCHAIN_FILE=build/Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build

source build/Release/generators/deactivate_conanbuild.sh
