Fast DDS 2.14.7 的官方文档未直接提供该精确版本的编译命令，但根据官方源码安装流程，核心步骤是**先编译 Foonathan Memory Vendor 和 Fast-CDR 依赖，再编译 Fast DDS 本体**。以下是通用命令，请将 `<version>` 替换为你实际使用的对应版本标签（如 `v2.2.7`）。

### 1. 准备工作
```bash
$ mkdir -p ~/Fast-DDS/install
$ cd ~/Fast-DDS
```

### 2. 编译 Foonathan Memory（内存分配器）
https://gitee.com/hexu1985/foonathan_memory_v0.7-3.git

```bash
$ git clone https://github.com/foonathan/memory.git
$ cd memory
$ git checkout v0.7-3
$ cmake -S. -Bbuild \
    -D FOONATHAN_MEMORY_BUILD_EXAMPLES=OFF \
    -D FOONATHAN_MEMORY_BUILD_TESTS=OFF \
    -D FOONATHAN_MEMORY_BUILD_TOOLS=ON \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo
$ cmake --build build -j4
$ cmake --install build --prefix ~/Fast-DDS/install
```

### 3. 编译 Fast-CDR（序列化库）
https://gitee.com/hexu1985/Fast-CDR.git
```bash
$ git clone https://github.com/eProsima/Fast-CDR.git
$ cd Fast-CDR
$ git checkout <version>  # 例如 v2.2.7
$ cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=RelWithDebInfo
$ cmake --build build -j4
$ cmake --install build --prefix ~/Fast-DDS/install
```

### 4. 编译 TinyXML2
https://gitee.com/hexu1985/Fast-CDR.git
```bash
$ git clone https://github.com/leethomason/tinyxml2.git
$ cd tinyxml2
$ git checkout 9.0.0
$ cmake -S. -Bbuild -DCMAKE_POSITION_INDEPENDENT_CODE=ON
$ cmake --build build
$ cmake --install build --prefix ~/Fast-DDS/install
```

### 5. 拷贝 asio 头文件
https://sourceforge.net/projects/asio/files/asio/1.18.1%20%28Stable%29/asio-1.18.1.tar.gz/download
复制asio头文件到~/Fast-DDS/install目录下( ASIO_VERSION 101801 // 1.18.1 )

### 6. 编译 Fast DDS 本体

https://gitcode.com/gh_mirrors/fa/Fast-DDS.git
```bash
$ git clone https://github.com/eProsima/Fast-DDS.git
$ cd Fast-DDS
$ git checkout <version>  # 例如 v2.14.7
$ mkdir build && cd build
$ cmake .. -DCMAKE_INSTALL_PREFIX=~/Fast-DDS/install
$ cmake --build . --target install
```

### 7. 环境配置（运行前必需）
```bash
export LD_LIBRARY_PATH=~/Fast-DDS/install/lib:$LD_LIBRARY_PATH
```

### 可用的 CMake 选项（可选）
编译 Fast DDS 时可按需追加以下标志：
- **`-DCOMPILE_EXAMPLES=ON`**：编译示例程序。
- **`-DSECURITY=ON`**：启用安全通信（需 OpenSSL）。
- **`-DPERFORMANCE_TESTS=ON`**：编译性能测试。

