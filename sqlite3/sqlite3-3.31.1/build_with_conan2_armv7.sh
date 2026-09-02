# 创建动态库版本
conan create . --user=baselib --channel=test -pr:b=default -pr:h=./profiles/raspberry --version=3.31.1 -o shared=True --build=missing

# 上传
conan upload sqlite3/3.31.1@baselib/test -r=artifactory --all

