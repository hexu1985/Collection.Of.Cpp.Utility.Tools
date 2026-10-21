Ubuntu 20.04 上编译安装 Fast DDS-Gen 的详细步骤：

fast-dds 2.14.7 对应的fast-cdr 版本 2.2.8，对应的fast-dds-gen版本 3.3.3

### **1. 安装依赖项**
首先安装必要的依赖工具和库：java and Gradle

### **2. 下载 Fast DDS-Gen 源码**
从 GitHub 克隆仓库并进入目录：
https://gitee.com/hexu1985/Fast-DDS-Gen.git
```bash
git clone https://github.com/eProsima/Fast-DDS-Gen.git
cd Fast-DDS-Gen
git checkout v3.3.3
```

### **3. 编译与安装**
使用 Gradle 构建项目：
```bash
gradle assemble
```

fast-dds-gen版本 3.3.3 有submodule依赖，如果访问不了外网，可以用国内镜像仓库，
具体的修改.gitmodules文件：

```bash
[submodule "thirdparty/idl-parser"]
    path = thirdparty/idl-parser
    url = https://gitee.com/hexu1985/IDL-Parser.git
[submodule "thirdparty/dds-types-test"]
    path = thirdparty/dds-types-test
    url = https://gitee.com/hexu1985/dds-types-test.git
```

然后执行git submodule同步
```bash
$ git submodule sync --recursive
$ git submodule update --init --recursive
```

编译完成后，生成的脚本和工具位于 `scripts` 目录下。为方便使用，建议将可执行文件路径加入系统环境变量：

```bash
# 临时生效（仅当前终端）
export PATH=$PATH:$(pwd)/scripts

# 永久生效（写入 ~/.bashrc）
echo "export PATH=\$PATH:$(pwd)/scripts" >> ~/.bashrc
source ~/.bashrc
```

### **4. 验证安装**
运行以下命令检查是否安装成功：
```bash
fastddsgen -version
```
如果输出类似 `fastddsgen version 2.2.0` 的版本信息，则说明安装成功。

---

---

## 在 Ubuntu 20.04 上安装 Gradle 的步骤如下：

### 1. 安装 JDK (Gradle 需要 Java 8 或更高版本)
```bash
sudo apt update
sudo apt install openjdk-11-jdk
```

验证 Java 安装：
```bash
java -version
```

### 2. 下载 Gradle
访问 [Gradle 官方发布页](https://gradle.org/releases/) 获取最新版本链接，然后使用 wget 下载：
```bash
wget https://services.gradle.org/distributions/gradle-7.6.4-bin.zip -P /tmp
```

### 3. 解压安装
```bash
sudo unzip -d /opt/gradle /tmp/gradle-*.zip
```

### 4. 配置环境变量
编辑 `~/.bashrc` 追加如下代码：
```bash
export PATH=$PATH:/opt/gradle/gradle-7.6.4/bin
```

### 5. 验证安装
```bash
gradle -v
```

### 注意事项
- 确保 `/opt/gradle` 有写入权限
- 如果遇到权限问题，可在解压时使用 `sudo`
- 对于生产环境，建议固定特定 Gradle 版本

