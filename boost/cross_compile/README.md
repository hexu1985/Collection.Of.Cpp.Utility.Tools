## ubuntu下交叉编译boost库

### 交叉编译器：
g++-arm-linux-gnueabihf、gcc-arm-linux-gnueabihf
```bash
$ sudo apt install gcc-arm-linux-gnueabihf
$ sudo apt install g++-arm-linux-gnueabihf
/usr/bin$ ls arm-linux*
arm-linux-gnueabihf-addr2line      arm-linux-gnueabihf-gcov-11
arm-linux-gnueabihf-ar             arm-linux-gnueabihf-gcov-dump
arm-linux-gnueabihf-as             arm-linux-gnueabihf-gcov-dump-11
arm-linux-gnueabihf-c++filt        arm-linux-gnueabihf-gcov-tool
arm-linux-gnueabihf-cpp            arm-linux-gnueabihf-gcov-tool-11
arm-linux-gnueabihf-cpp-11         arm-linux-gnueabihf-gprof
arm-linux-gnueabihf-dwp            arm-linux-gnueabihf-ld
arm-linux-gnueabihf-elfedit        arm-linux-gnueabihf-ld.bfd
arm-linux-gnueabihf-g++            arm-linux-gnueabihf-ld.gold
arm-linux-gnueabihf-g++-11         arm-linux-gnueabihf-lto-dump-11
arm-linux-gnueabihf-gcc            arm-linux-gnueabihf-nm
arm-linux-gnueabihf-gcc-11         arm-linux-gnueabihf-objcopy
arm-linux-gnueabihf-gcc-ar         arm-linux-gnueabihf-objdump
arm-linux-gnueabihf-gcc-ar-11      arm-linux-gnueabihf-ranlib
arm-linux-gnueabihf-gcc-nm         arm-linux-gnueabihf-readelf
arm-linux-gnueabihf-gcc-nm-11      arm-linux-gnueabihf-size
arm-linux-gnueabihf-gcc-ranlib     arm-linux-gnueabihf-strings
arm-linux-gnueabihf-gcc-ranlib-11  arm-linux-gnueabihf-strip
arm-linux-gnueabihf-gcov
/usr/arm-linux-gnueabihf$ ls
bin  include  lib
```

### boost版本：
boost-1.65.1
```bash
# 去https://www.boost.org/releases/1.65.1/下载boost_1_65_0.tar.gz
$ tar xzvf boost_1_65_1.tar.gz
```

### 编译boost

**1. 初始化构建系统**
进入解压好的 Boost 1.65.1 源码目录，运行引导脚本：
```bash
./bootstrap.sh
```
这会生成 `b2` 可执行文件和 `project-config.jam` 配置文件。

**2. 关键一步：配置工具链**
为了让 Boost 知道该用哪个编译器，你需要编辑Boost 源码目录下`project-config.jam` 文件，在里面添加一行：
```
using gcc : arm : arm-linux-gnueabihf-g++ ;
```
这定义了一个名为 `arm` 的工具集，它指向你的交叉编译器。

**3. 执行编译**
回到 Boost 源码目录，运行 `b2` 命令。你可以根据需求选择生成静态库或动态库：

*   **生成动态库（`.so` 文件）**：
    ```bash
    ./b2 --toolset=gcc-arm \
         target-os=linux \
         architecture=arm \
         threading=multi \
         link=shared \
         stage
    ```
*   **生成静态库（`.a` 文件）**：
    将上面的 `link=shared` 改为 `link=static` 即可。

**4. 验证结果**
编译完成后，库文件会生成在源码目录下的 `stage/lib` 文件夹里。你可以用 `file` 命令来验证它确实是 ARM 架构的：
```bash
file stage/lib/libboost_system.so.1.65.0
```
输出里应该包含 `ELF 32-bit LSB ... ARM` 之类的字样。


