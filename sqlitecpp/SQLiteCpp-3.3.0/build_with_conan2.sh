# 创建包
conan create . --version=3.3.0 \
    -o shared=True \
    -o internal_sqlite=False \
    -o build_examples=False \
    -o build_tests=False \
    -o use_asan=False \
    -o use_gcov=False \
    --build=missing \
    -r artifactory

# 指定user和channel
conan create . --user=baselib --channel=test --version=3.3.0 \
    -o shared=True \
    -o internal_sqlite=False \
    -o build_examples=False \
    -o build_tests=False \
    -o use_asan=False \
    -o use_gcov=False \
    --build=missing \
    -r artifactory

# 查看已配置的远程仓库
conan remote list

# 如果没有配置，先添加
conan remote add jfrog https://your-jfrog-server/artifactory/api/conan/conan-local

# 如果需要认证
conan remote login jfrog your_username -p your_password

# 上传
conan upload sqlitecpp/3.3.0 -r=artifactory --all

# 上传
conan upload sqlitecpp/3.3.0@baselib/test -r=artifactory --all

