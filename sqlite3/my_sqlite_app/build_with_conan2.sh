
# 添加JFrog远程仓库
#conan remote add jfrog https://your-jfrog-server/artifactory/api/conan/conan-local

# 如果需要认证
#conan remote login jfrog your_username -p your_password

#conan build . --build=missing -r artifactory
#conan build . -r artifactory

mkdir build && cd build
conan install ..
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake
cmake --build .
