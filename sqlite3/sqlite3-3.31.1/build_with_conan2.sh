# 创建包（不包含shell）
conan create . --version=3.31.1 --build=missing

# 创建包（包含shell）
conan create . --version=3.31.1 -o build_shell=True --build=missing

# 创建动态库版本
conan create . --version=3.31.1 -o shared=True --build=missing

# 上传
conan upload sqlite3/3.31.1 -r=artifactory --all

# 创建动态库版本
conan create . --user=baselib --channel=test --version=3.31.1 -o shared=True --build=missing

# 上传
conan upload sqlite3/3.31.1@baselib/test -r=artifactory --all

