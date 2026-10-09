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
boost-1.65.0
```bash
# 去https://www.boost.org/releases/1.65.0/下载boost_1_65_0.tar.gz
$ tar xzvf boost_1_65_0.tar.gz
```

### 编译boost

**1. 初始化构建系统**
进入解压好的 Boost 1.65.0 源码目录，运行引导脚本：
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


---

**5. 编译动态库并安装到指定目录**

核心是在 `b2` 命令中组合使用 `link=shared` 和 `--prefix` 参数。

在你的 Boost 源码目录下，运行：

```bash
./b2 install \
    toolset=gcc-arm \
    target-os=linux \
    architecture=arm \
    threading=multi \
    link=shared \
    --prefix=/你的/指定/安装目录
```

**参数说明**

*   **`install`**：这是 `b2` 的一个**目标（target）**，表示将编译产物（头文件和库）安装到系统目录结构中。
*   **`link=shared`**：指定生成**动态库**（`.so` 文件）。如果你需要静态库，则应改为 `link=static`。
*   **`--prefix=/你的/指定/安装目录`**：指定安装的根路径。Boost 的头文件会被安装到 `<PREFIX>/include`，库文件默认安装到 `<PREFIX>/lib`。这是与 `stage`（仅将库文件复制到源码目录下的 `stage` 文件夹）最本质的区别。

**验证安装**

安装完成后，你可以用 `file` 命令检查生成的 `.so` 文件是否为 ARM 架构：

```bash
file /你的/指定/安装目录/lib/libboost_system.so.1.65.0
```

输出中应包含 `ELF 32-bit LSB shared object, ARM` 等字样，以确认交叉编译成功。

---

**6. 编译和安装是分开两步执行**

Boost 的构建系统 `b2` 支持分步操作。你可以先执行一次 `b2` 命令来仅完成编译，生成的库文件默认会存放在源码目录下的 `stage/lib` 文件夹中。然后，再单独执行一次 `b2 install` 命令，专门负责将已编译好的库文件以及头文件复制到你指定的安装目录。

以下是针对你的 ARM 交叉编译场景，将编译和安装分开的示例：

**第一步：仅编译**
```bash
# 在 Boost 源码根目录下执行
./b2 \
    toolset=gcc-arm \
    target-os=linux \
    architecture=arm \
    threading=multi \
    link=shared
```
执行完毕后，ARM 架构的动态库（`.so` 文件）会生成在 `stage/lib` 目录下。

**第二步：仅安装**
```bash
./b2 install \
    toolset=gcc-arm \
    target-os=linux \
    architecture=arm \
    threading=multi \
    link=shared \
    --prefix=/你的/指定/安装目录
```
这条命令会读取 `stage/lib` 里已有的编译产物（如果尚未编译，它会先触发编译），并将它们和头文件一起安装到 `--prefix` 指定的路径下。

**📝 核心区别与建议**

分开操作的核心区别在于 **“编译”与“安装”的侧重点不同**：

*   **`./b2`（无 install/stage 目标）**：主要进行**编译**，产物默认放在源码树内的 `bin.v2` 或 `stage` 目录。适合你只想确认编译是否成功，暂时不想污染系统目录的场景。
*   **`./b2 install`**：主要进行**安装**。它会把文件复制到 `--prefix` 指定的**独立目录**，该目录结构清晰（`include` 和 `lib`），适合作为最终的交付或部署路径。
*   **`./b2 stage`**：这是一个更纯粹的“编译并存放”目标。它只编译库文件并放到 `stage` 目录，**不处理头文件**，也不涉及 `--prefix` 指定的独立安装目录。

**建议**：如果你需要反复调试编译过程，分开执行更清晰；如果只是想一次性完成，合并执行也无妨。但如果你需要验证编译结果，或者库文件很大、安装到不同位置，分开操作会更灵活。

---

**7. 删除上次编译的结果**

调整编译选项后要删除上次的编译结果，最直接的办法就是删掉 `bin.v2` 这个中间产物目录。

**🗑️ 推荐方法：直接删除 `bin.v2`**

`b2` 会把所有的中间文件（`.o` 等）默认放在源码目录下的 `bin.v2` 文件夹里。直接删掉它，就能确保下次编译完全从头开始，不会复用任何旧的缓存。

```bash
rm -rf bin.v2
```

**⚠️ 关于 `b2 --clean` 的提醒**

Boost 也提供了 `--clean` 参数，但**不推荐**用它来清理交叉编译的缓存。因为它高度依赖你传入的属性必须和上次编译时**完全一致**，否则它会找不到该清理什么，看起来就像没生效一样。

而你调整了编译选项，属性必然发生了变化，用 `--clean` 很容易“擦不干净”。所以，直接 `rm -rf bin.v2` 是最省心、最可靠的做法。
