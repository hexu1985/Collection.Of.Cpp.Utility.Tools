### abseil-cpp开源项目

克隆项目：
```
$ git clone https://github.com/abseil/abseil-cpp.git
```

项目地址: <https://github.com/abseil/abseil-cpp>

编译安装项目（基于cmake）

```shell
cmake -S . -B build -DCMAKE_PREFIX_PATH=~/local \
    -DCMAKE_INSTALL_PREFIX=~/local -DABSL_ENABLE_INSTALL=ON \
    -DABSL_USE_EXTERNAL_GOOGLETEST=ON -DABSL_FIND_GOOGLETEST=ON
cmake --build build
cmake --build build --target install
```
