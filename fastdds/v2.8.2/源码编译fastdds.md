# 源码编译fastdds，且源码依赖

1. 准备工作

在$HOME/下建立fast_dds目录
cd到fast_dds目录

```bash
$ mkdir ~/fast_dds/
$ cd ~/fast_dds/
```

2. clone fastcdr项目，源码编译
https://gitee.com/hexu1985/fast-cdr-v1.0.28.git
```bash
$ cd ~/fast_dds
$ git clone https://github.com/eProsima/Fast-CDR.git
$ cd Fast-CDR   # change branch to 1.0.x
$ git checkout v1.0.28
$ cmake -S. -Bbuild
$ cmake --build build
$ cmake --install build --prefix ~/fast_dds/install
```

3. 复制asio头文件到~/fast_dds/install/include目录下( ASIO_VERSION 101801 // 1.18.1 )

4. clone foonathan_memory项目，源码编译
https://gitee.com/hexu1985/foonathan_memory_v0.7-3.git

```bash
$ cd ~/fast_dds
$ git clone https://github.com/foonathan/memory.git
$ cd memory
$ git checkout v0.7-3
$ cmake -S. -Bbuild
$ cmake --build build
$ cmake --install build --prefix ~/fast_dds/install
```

5. clone tinyxml2项目，源码编译
https://gitee.com/hexu1985/tinyxml2_v9.0.0.git
```bash
$ cd ~/fast_dds
$ git clone https://github.com/leethomason/tinyxml2.git
$ cd tinyxml2
$ git checkout 9.0.0
$ cmake -S. -Bbuild -DCMAKE_POSITION_INDEPENDENT_CODE=ON
$ cmake --build build
$ cmake --install build --prefix ~/fast_dds/install
```

必须指定CMAKE_POSITION_INDEPENDENT_CODE=ON打开-fPIC编译选项，不然libfastdds.so链接libtinyxml2.a时会报如下错误：

```
[ 99%] Linking CXX shared library libfastrtps.so
/usr/bin/ld: /home/hexu/fast_dds/install/lib/libtinyxml2.a(tinyxml2.cpp.o): relocation R_X86_64_PC32 against symbol `stdout@@GLIBC_2.2.5' can not be used when making a shared object; recompile with -fPIC
/usr/bin/ld: final link failed: bad value
collect2: error: ld returned 1 exit status
make[2]: *** [src/cpp/CMakeFiles/fastrtps.dir/build.make:3267: src/cpp/libfastrtps.so.2.8.2] Error 1
make[1]: *** [CMakeFiles/Makefile2:980: src/cpp/CMakeFiles/fastrtps.dir/all] Error 2
make: *** [Makefile:141: all] Error 2
```

6. 源码编译fastdds
https://gitee.com/hexu1985/fast-dds-v2.8.2.git
```bash
$ cd ~/fast_dds
$ git clone https://github.com/eProsima/Fast-DDS.git
$ cd Fast-DDS # change to branch 2.8.2
$ cmake -S. -Bbuild -DCMAKE_INSTALL_PREFIX=~/fast_dds/install
$ cmake --build build
$ cmake --install build --prefix ~/local/fast_dds
```

注释：eprosima_find_package会自动去支持从特定路径（如 eProsima 的本地安装目录）查找依赖例如~/fast_dds/install

然后所有的头文件和库都被安装到~/fast_dds/install目录下。

# 源码编译fastdds的单元测试

在前面5个步骤的基础上，需要编译googletest

6. 源码编译googletest

fastdds v2.8.2版本依赖googletest的v1.13.x版本

```
$ cd ~/fast_dds
$ git clone https://github.com/google/googletest.git
$ cd googletest
$ git checkout v1.13.x
$ cmake -S. -Bbuild
$ cmake --build build --parallel 4
$ cmake --install build --prefix ~/fast_dds/install
```

7. 编译fastdds单元测试

```bash
$ cd ~/fast_dds
$ cd Fast-DDS # change to branch 2.8.2
$ cmake -S. -Bbuild -DCMAKE_INSTALL_PREFIX=~/fast_dds/install \
    -DEPROSIMA_BUILD_TESTS=ON -DFASTRTPS_API_TESTS=ON \
    -DGTest_ROOT=~/fast_dds/install -DAsio_INCLUDE_DIR=~/fast_dds/asio-1.18.1/include
$ cmake --build build -j8
```


