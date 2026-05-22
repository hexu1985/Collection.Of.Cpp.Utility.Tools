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
$ sh cuda_11.4.4_470.82.01_linux.run --toolkit --silent --toolkitpath=/home/hexu/local/cuda-11.4    # 指定路径
```

可以，我们可以把所有东西都装在你指定的目录里。

这主要通过两种方式实现：一是 **CUDA Toolkit 官方支持 Runfile 安装的 `--toolkitpath` 参数**，你可以直接把 CUDA 主体装到自定义目录；二是 **cuDNN 和 TensorRT 的 `.tar` 包**，它们可以解压到任何位置，实现了“安装即文件夹”。下面我按照“编译环境”的需求，结合你的 Ubuntu 20.04，一步步来实现。

> 核心思路：把 CUDA 装到自定义路径（比如 `/home/user/tools/cuda-11.8`），再把 cuDNN 和 TensorRT 的库文件放到这个 CUDA 路径里，最后配置好环境变量让编译工具能识别它们。

### 📁 环境概览与规划

假设我们将所有工具都安装在用户主目录下的 `tools` 文件夹中，方便统一管理。你规划时按自己想法调整就好。

| 工具 | 版本示例 | 安装方式 | 自定义路径示例 |
| :--- | :--- | :--- | :--- |
| **CUDA** | 11.8 | Runfile 安装 (`--toolkitpath`) | `/home/user/tools/cuda-11.8` |
| **cuDNN** | 8.9.7 | Tar 包解压并拷贝 | (解压到 CUDA 路径下) |
| **TensorRT** | 8.6.1 | Tar 包解压 | `/home/user/tools/TensorRT-8.6.1` |

### 📦 第一步：将 CUDA Toolkit 安装到指定路径

CUDA 提供了 Runfile 安装器，最适合自定义目录安装，步骤如下：

1.  **下载 Runfile 安装包**：从 [NVIDIA 官网](https://developer.nvidia.com/cuda-toolkit-archive) 选择符合你需求的 CUDA 版本（比如 11.8），然后下载 Runfile 文件。请务必确认版本与你的项目需求匹配。
2.  **执行无头安装**：这是最关键的一步。导航到下载目录，运行以下命令跳过安装向导，直接将 Toolkit 安装到指定路径：
    ```bash
    # 将 "11.8" 和 "/path/to/your/cuda-11.8" 替换为你实际的版本号和路径
    sh cuda_11.8.0_520.61.05_linux.run --toolkit --silent --toolkitpath=/path/to/your/cuda-11.8
    ```
    
    此命令会无交互地进行静默安装。  
    **附加说明**：
    *   这里我们忽略了 Driver 和 Samples，因为编译环境只需要 Toolkit 就够了。
    *   如果下载的 CUDA 版本与当前系统驱动不兼容，安装器会报错。建议使用 `nvidia-smi` 查看驱动支持的 CUDA 版本上限，选择一个不高于它的 CUDA Toolkit 版本。
3.  **配置 CUDA 环境变量**：为了让系统在编译时能找到刚装的 CUDA，需要设置环境变量。将以下内容添加到 `~/.bashrc` 文件末尾：
    ```bash
    export PATH=/path/to/your/cuda-11.8/bin:$PATH
    export LD_LIBRARY_PATH=/path/to/your/cuda-11.8/lib64:$LD_LIBRARY_PATH
    export CUDA_HOME=/path/to/your/cuda-11.8
    ```

    **非 root 用户特别注意**：许多服务器上普通用户对 `/usr/local` 没有写入权限，但依然可以用这种方法将 CUDA 安装到自己的 `home` 目录下，非常方便。

### 🔗 第二步：将 cuDNN 集成到 CUDA 目录

cuDNN 的安装比较简单，本质是把它的库文件和头文件复制到 CUTA 目录下即可。NVIDIA 官方推荐使用 `.tar` 包安装。

1.  **下载 cuDNN 的 Tar 包**：从 [NVIDIA 开发者网站](https://developer.nvidia.com/cudnn) 下载与 CUDA 版本（如 11.8）匹配的 **cuDNN Library for Linux (x86_64)**。
2.  **解压并复制文件**：解压后，将 `cuda` 文件夹下的 `include` 和 `lib` 文件夹内容，分别复制到你 CUDA 安装目录的对应文件夹中。
    ```bash
    # 1. 解压
    tar -xzvf cudnn-linux-x86_64-8.9.7.29_cuda11-archive.tar.xz

    # 2. 复制头文件和库文件
    cp cudnn-linux-x86_64-8.9.7.29_cuda11-archive/include/cudnn*.h /path/to/your/cuda-11.8/include/
    cp -P cudnn-linux-x86_64-8.9.7.29_cuda11-archive/lib/libcudnn* /path/to/your/cuda-11.8/lib64/
    
    # 3. 修改文件权限（可选，确保可读）
    chmod a+r /path/to/your/cuda-11.8/include/cudnn*.h /path/to/your/cuda-11.8/lib64/libcudnn*
    ```

### 🚀 第三步：安装 TensorRT 到指定目录

1.  **下载 TensorRT 的 Tar 包**：从 [NVIDIA 开发者网站](https://developer.nvidia.com/tensorrt) 选择与 CUDA 版本（如 11.8）匹配的 **TensorRT Tar Package** 下载。
2.  **解压到指定目录**：Tar 包解压后自动创建一个版本子文件夹，非常干净。
    ```bash
    mkdir -p /path/to/your/tensorrt
    tar -xzvf TensorRT-8.6.1.6.Linux.x86_64-gnu.cuda-11.8.cudnn8.9.tar.gz -C /path/to/your/tensorrt
    ```
    解压完成后，你的 TensorRT 主目录在 `/path/to/your/tensorrt/TensorRT-8.6.1.6`。为方便路径管理，可以建立一个不带版本号的软链接：
    ```bash
    ln -s /path/to/your/tensorrt/TensorRT-8.6.1.6 /path/to/your/tensorrt/trt
    ```
3.  **配置 TensorRT 环境变量**：为了让编译器能找到 TensorRT 的头文件和库，需要设置 `LD_LIBRARY_PATH`。此外，很多 **CMake 项目会通过查找 `TensorRT_ROOT` 环境变量来定位 TensorRT**，所以建议一并设置。将以下内容添加到 `~/.bashrc`：
    ```bash
    export LD_LIBRARY_PATH=/path/to/your/tensorrt/TensorRT-8.6.1.6/lib:$LD_LIBRARY_PATH
    export TensorRT_ROOT=/path/to/your/tensorrt/TensorRT-8.6.1.6
    ```

### 🔧 第四步：验证安装

完成以上步骤，最后记得运行 `source ~/.bashrc` 让环境变量生效。然后就可以验证了：

*   **验证 CUDA**：
    ```bash
    nvcc --version
    which nvcc
    ```
*   **验证 cuDNN**：
    ```bash
    cat /path/to/your/cuda-11.8/include/cudnn_version.h | grep CUDNN_MAJOR -A 2
    ```
*   **验证 TensorRT**：
    ```bash
    /path/to/your/tensorrt/TensorRT-8.6.1.6/bin/trtexec --version
    ```
*   **验证编译环境链接**：这是为了确保你的编译环境能正确找到所有库。如果你用 CMake，可以在 `CMakeLists.txt` 中通过 `-DCUDA_TOOLKIT_ROOT_DIR`、`-DCUDNN_ROOT_DIR` 和 `-DTENSORRT_ROOT_DIR` 分别指定三方库路径。用一个简单的 `CMakeLists.txt` 测试一下通常比手动编译更准确。

这个流程同样适用于为自己的用户安装多个版本的 CUDA。你可以在 `home` 目录下建立 `cuda-11.8`、`cuda-12.0` 等不同文件夹，然后在 `~/.bashrc` 中通过修改环境变量来轻松切换。