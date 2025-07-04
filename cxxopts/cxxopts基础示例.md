以下是 **cxxopts** 的详细示例代码，展示如何定义、解析和使用命令行参数：

---

### **基础示例**
```cpp
#include <iostream>
#include <string>
#include <cxxopts.hpp>  // 需要包含头文件

int main(int argc, char** argv) {
    // 1. 创建 Options 对象（程序名和描述）
    cxxopts::Options options("MyProgram", "A simple example of cxxopts");

    // 2. 添加命令行选项
    options.add_options()
        // 无参选项（如 --help）
        ("h,help", "Print help")
        // 带参选项（值类型为 std::string）
        ("f,file", "Input file path", cxxopts::value<std::string>())
        // 带默认值的选项
        ("d,debug", "Enable debug mode", cxxopts::value<bool>()->default_value("false"))
        // 多值选项（如 --vec 1 2 3）
        ("v,vec", "Vector of numbers", cxxopts::value<std::vector<int>>());

    // 3. 解析命令行参数
    try {
        auto result = options.parse(argc, argv);

        // 4. 处理选项
        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }

        if (result.count("file")) {
            std::string file = result["file"].as<std::string>();
            std::cout << "Input file: " << file << std::endl;
        }

        bool debug = result["debug"].as<bool>();
        if (debug) {
            std::cout << "Debug mode enabled!" << std::endl;
        }

        if (result.count("vec")) {
            auto vec = result["vec"].as<std::vector<int>>();
            std::cout << "Vector: ";
            for (int num : vec) std::cout << num << " ";
            std::cout << std::endl;
        }

    } catch (const cxxopts::exceptions::exception& e) {
        // 捕获解析错误（如缺少必需参数、类型不匹配）
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << options.help() << std::endl;
        return 1;
    }

    return 0;
}
```

---

### **编译与运行**
假设代码保存为 `main.cpp`，编译时需确保 `cxxopts.hpp` 在包含路径中：
```bash
g++ -std=c++11 main.cpp -o myprogram
```

**运行示例：**
```bash
# 查看帮助
./myprogram --help

# 正常使用
./myprogram --file data.txt --debug --vec 1 2 3

# 错误处理（缺少必需参数或类型错误）
./myprogram --file 123 --debug=invalid
```

---

### **关键功能说明**
1. **选项定义**：
   - 使用 `add_options()` 链式调用定义参数。
   - 支持短选项（`-f`）和长选项（`--file`）。
   - 通过 `cxxopts::value<T>()` 指定参数类型（如 `std::string`、`bool`、`std::vector`）。

2. **参数获取**：
   - `result.count("key")`：检查选项是否存在。
   - `result["key"].as<T>()`：获取选项的值（需指定类型 `T`）。

3. **错误处理**：
   - 自动生成帮助信息（`options.help()`）。
   - 捕获 `cxxopts::exceptions` 处理非法输入。

4. **高级功能**：
   - 设置必需参数：`.->required()`。
   - 自定义验证器：通过 `->check()` 添加条件检查。

---

### **更多用法**
- **必需参数**：
  ```cpp
  options.add_options()
      ("i,input", "Input file (required)", cxxopts::value<std::string>()->required());
  ```

- **位置参数**：
  ```cpp
  options.parse_positional({"input"});
  ```

- **默认值**：
  ```cpp
  ("port", "Port number", cxxopts::value<int>()->default_value("8080"));
  ```

cxxopts 的简洁 API 和类型安全特性使其成为 C++ 命令行解析的优质选择。
