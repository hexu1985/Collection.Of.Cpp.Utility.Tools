## 安装cuda toolkit

### 🌐 通过 NVIDIA 官网查找
如果你想从官网页面手动下载，可以按以下步骤操作：
1.  访问 NVIDIA 的 **CUDA 历史版本存档页面**：[https://developer.nvidia.com/cuda-toolkit-archive](https://developer.nvidia.com/cuda-toolkit-archive)
2.  在列表中找到并点击 **"CUDA Toolkit 11.0"**（2020年8月发布）。
3.  进入下载页面后，根据你的系统配置选择：
    *   **Operating System**: Linux
    *   **Architecture**: x86_64
    *   **Distribution**: Ubuntu（或你的具体发行版）
    *   **Version**: 根据你的系统版本选择（如20.04/18.04）
    *   **Installer Type**: runfile (local)
4.  页面会生成对应的下载链接和安装说明，你可以使用`wget`命令或直接点击链接进行下载。

### ⚠️ 重要安装提示
下载完成后，在安装时有几个关键点需要注意，否则容易报错：
*   **执行权限**：下载后需要先给文件添加执行权限，再运行安装。建议使用以下命令，避免官网 `sudo sh` 命令可能带来的问题：
    ```bash
    chmod +x cuda_11.0.3_450.51.06_linux.run
    sudo ./cuda_11.0.3_450.51.06_linux.run
    ```
*   **取消勾选驱动**：进入安装界面后，务必在组件列表中**取消勾选 "Driver"** 选项。因为安装包里自带的驱动（450.51.06）可能与系统现有的驱动冲突，独立安装驱动是更稳妥的做法。
*   **阅读协议**：在安装过程中，需要输入 `accept` 来确认用户许可证协议，然后才能继续安装。


```bash
$ wget https://developer.download.nvidia.com/compute/cuda/11.4.4/local_installers/cuda_11.4.4_470.82.01_linux.run
$ sudo sh cuda_11.4.4_470.82.01_linux.run
```
