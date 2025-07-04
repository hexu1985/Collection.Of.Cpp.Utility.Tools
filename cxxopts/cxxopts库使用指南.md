# cxxopts 库使用说明

cxxopts 是一个轻量级的 C++ 命令行参数解析库，具有简单易用的 API 和良好的类型安全性。

## 基本特性

- 仅头文件库，易于集成
- 支持多种参数类型（布尔、数值、字符串等）
- 自动生成帮助信息
- 支持位置参数和可选参数
- 支持参数分组和复杂验证

## 安装方法

1. 直接从 GitHub 下载头文件：https://github.com/jarro2783/cxxopts
2. 或者使用包管理器：
   - vcpkg: `vcpkg install cxxopts`
   - Conan: `conan install cxxopts/3.0.0`

## 基本用法

```cpp
#include <cxxopts.hpp>
#include <iostream>

int main(int argc, char** argv) {
    cxxopts::Options options("MyProgram", "A brief description of my program");

    // 添加选项
    options.add_options()
        ("h,help", "Print help")
        ("i,input", "Input file", cxxopts::value<std::string>())
        ("o,output", "Output file", cxxopts::value<std::string>()->default_value("output.txt"))
        ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))
        ("n,number", "Number of iterations", cxxopts::value<int>()->default_value("10"))
        ("d,debug", "Enable debugging", cxxopts::value<bool>()->implicit_value("true"))
    ;

    try {
        // 解析命令行参数
        auto result = options.parse(argc, argv);

        // 处理帮助选项
        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }

        // 获取参数值
        std::string input_file = result["input"].as<std::string>();
        std::string output_file = result["output"].as<std::string>();
        bool verbose = result["verbose"].as<bool>();
        int iterations = result["number"].as<int>();
        bool debug = result["debug"].as<bool>();

        // 使用参数...
        
    } catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

## 高级特性

### 位置参数

```cpp
options.add_options()
    ("positional", "Positional arguments", cxxopts::value<std::vector<std::string>>())
;

options.parse_positional({"positional"});

auto result = options.parse(argc, argv);
auto positional_args = result["positional"].as<std::vector<std::string>>();
```

### 参数组

```cpp
options.add_options("Group 1")
    ("a,option-a", "Option A", cxxopts::value<int>())
;

options.add_options("Group 2")
    ("b,option-b", "Option B", cxxopts::value<std::string>())
;
```

### 参数验证

```cpp
options.add_options()
    ("n,number", "A number between 1 and 10", 
     cxxopts::value<int>()->default_value("5")->check([](int n) {
         if (n < 1 || n > 10) {
             return "must be between 1 and 10";
         }
         return "";
     }))
;
```

### 复杂类型

```cpp
// 自定义类型解析
struct Point {
    int x, y;
};

namespace cxxopts {
    template<>
    class value<Point> : public value_base {
    public:
        value() : value_base() {}
        
        void parse(const std::string& text) override {
            std::istringstream iss(text);
            char comma;
            if (!(iss >> m_value.x >> comma >> m_value.y) || comma != ',') {
                throw_or_mimic<argument_incorrect_type>(text);
            }
        }
        
        const Point& value() const { return m_value; }
        
    private:
        Point m_value;
    };
}

// 使用自定义类型
options.add_options()
    ("p,point", "A point (x,y)", cxxopts::value<Point>())
;
```

## 常见问题

1. **如何处理必需参数？**

   ```cpp
   if (!result.count("input")) {
       std::cerr << "Input file is required" << std::endl;
       return 1;
   }
   ```

2. **如何设置多值参数？**

   ```cpp
   options.add_options()
       ("f,file", "Input files", cxxopts::value<std::vector<std::string>>())
   ;
   ```

3. **如何隐藏某些选项的帮助信息？**

   ```cpp
   options.add_options()
       ("s,secret", "Hidden option", cxxopts::value<std::string>()->help(""))
   ;
   ```

## 更多信息

- GitHub 仓库: https://github.com/jarro2783/cxxopts
- 文档: https://github.com/jarro2783/cxxopts/wiki

cxxopts 是一个简单而强大的库，适合大多数命令行应用程序的需求。
