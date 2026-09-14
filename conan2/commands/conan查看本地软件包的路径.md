conan查看本地的安装的sqlite3库软件包的路径？

---

要查看本地安装的 SQLite3 库软件包的路径，最直接的方法是使用 **`conan cache path`** 命令。

### 💻 核心命令
```bash
conan cache path sqlite3/<版本号>:<Package_ID>
```
如果不确定具体的 `Package_ID`，Conan 会默认使用最新版本和最新的二进制包。你也可以简化命令，只指定包名和版本：
```bash
conan cache path sqlite3/3.45.0
```

### 📝 操作步骤
由于你本地可能安装了多个不同架构的 SQLite3 版本，建议先确认你要找的具体包标识，再获取路径。

**1. 先确认本地的包信息**
使用 `conan list` 命令查看本地缓存中 SQLite3 的所有版本和对应的 `Package_ID`：
```bash
conan list "sqlite3/*:*"
```
在输出结果中，你可以根据之前提到的 `arch` 设置（如 `x86_64` 或 `armv8`），找到你需要的那个 `Package_ID`。

**2. 获取对应的安装路径**
拿到具体的 `Package_ID` 后，将其拼接到引用后执行：
```bash
conan cache path sqlite3/3.45.0#<你找到的Package_ID>
```
命令执行后，终端会直接输出该二进制包在你本地缓存中的根目录路径（通常以 `.../p/<hash>/p` 结尾）。

### ⚠️ 重要提醒
你获取到的路径是 Conan 缓存内部的存储位置。**Conan 官方建议将此缓存目录视为只读**，不要直接修改、删除或添加其中的文件，以免破坏包的完整性。如果你需要将库文件（如 `.lib` 或 `.a`）用于其他项目，建议使用 `conan install` 将其部署到指定目录，而不是直接从缓存中复制。
