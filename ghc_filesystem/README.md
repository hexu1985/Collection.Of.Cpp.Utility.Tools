
`ghc::filesystem` 是一个为 **C++11/14** 环境提供的、与 C++17 标准 `<filesystem>` 保持高度兼容的**单头文件**库。当你的项目由于编译器版本限制无法使用标准的 `std::filesystem` 时，它是一个非常理想的替代方案。

### 📦 主要特点

*   **标准兼容的API**：接口和功能严格遵循 C++17 标准，在支持C++17标准库的项目中，它可以作为一个直接的、即插即用的替代品。
*   **独立命名空间**：所有实现都位于 `ghc::filesystem` 命名空间下，避免了与 `std::filesystem` 冲突，甚至允许你在同一项目中混合使用两者。
*   **跨平台支持**：在 Windows、macOS、Linux、FreeBSD 等多个主流操作系统上经过测试，也提供了对 Android NDK、Emscripten 和 QNX 的实验性支持。
*   **多种使用模式**：最推荐的是**仅头文件模式**，直接包含 `ghc/filesystem.hpp` 即可。也支持**声明与实现分离的模式**（通过 `ghc/fs_fwd.hpp` 和 `ghc/fs_impl.hpp`）来减少头文件依赖。

### 🚀 快速上手

使用 `ghc::filesystem` 非常简单。你只需要将它的头文件复制到你的项目中，包含进来，并定义一个命名空间别名即可：

```cpp
#include "ghc/filesystem.hpp"
namespace fs = ghc::filesystem;

int main() {
    // 创建目录
    fs::create_directories("data/images");

    // 检查文件是否存在
    if (fs::exists("source.txt")) {
        // 复制文件
        fs::copy_file("source.txt", "data/backup.txt");
    }

    // 遍历目录
    for (auto& entry : fs::directory_iterator("data")) {
        if (fs::is_regular_file(entry.status())) {
            // do something
        }
    }
}
```
这段代码演示了它和标准库几乎一致的用法。由于它是一个头文件库，无需编译链接，集成到项目中非常便捷。

### 💡 何时使用

*   当你使用的编译器不完全支持 C++17 的 `<filesystem>` 时。
*   当你需要保持代码与旧标准兼容，但又想使用现代的文件系统操作API时。
*   作为一个轻量级、易集成的替代方案，可以避免引入 Boost 这样庞大的库。


### 仓库链接：https://github.com/gulrak/filesystem.git
