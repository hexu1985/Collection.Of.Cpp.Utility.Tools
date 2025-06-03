fast-dds文档
https://fast-dds.docs.eprosima.com/en/v2.8.2/

4.1.1. 了解FastDDS、FastRTPS 的底层实现原理
https://www.yuque.com/yuefeng-95ahz/blogs/hm3a7zg8zvnoc2wk?singleDoc#

4.1.2. DDS 通用优化方案
https://www.yuque.com/yuefeng-95ahz/blogs/greids1v41m78kc4#

RTPS协议（一）概述
https://zhuanlan.zhihu.com/p/587446620

DDS通信中间件——RTPS规范
https://zhuanlan.zhihu.com/p/706886784

FastDDS中的QoS
https://blog.csdn.net/nuist_NJUPT/article/details/129745123

1. Getting Started
https://fast-dds.docs.eprosima.com/en/v2.8.2/fastdds/getting_started/definitions.html

3. DDS Layer
https://fast-dds.docs.eprosima.com/en/v2.8.2/fastdds/dds_layer/dds_layer.html#dds-layer

分布式实时通信——DDS技术
https://zhuanlan.zhihu.com/p/192981171

DDS-RTPS协议权威指南
https://wenku.csdn.net/column/5civg171f1#1.%20DDS-RTPS%E5%8D%8F%E8%AE%AE%E6%A6%82%E8%BF%B0

一文了解DDS实时传输协议及其消息格式
https://blog.csdn.net/usstmiracle/article/details/134506674


# 源码编译fastdds，且源码依赖

1. 准备工作

在$HOME/下建立fast_dds目录
cd到fast_dds目录

```bash
$ mkdir ~/fast_dds/
$ cd ~/fast_dds/
```

2. clone fastcdr项目，源码编译

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

```bash
$ cd ~/fast_dds
$ git clone https://github.com/eProsima/Fast-DDS.git
$ cd Fast-DDS # change to branch 2.8.2
$ cmake -S. -Bbuild -DCMAKE_INSTALL_PREFIX=~/fast_dds/install
$ cmake --build build
$ cmake --install build --prefix ~/local/fast_dds
```

注释：eprosima_find_package会自动去支持从特定路径（如 eProsima 的本地安装目录）查找依赖，例如~/fast_dds/install

然后所有的头文件和库都被安装到~/fast_dds/install目录下。

