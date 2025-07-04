#include <iostream>
#include <string>
#include "cxxopts.hpp"  // 需要包含头文件

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
