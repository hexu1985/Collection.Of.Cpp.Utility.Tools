#include <iostream>
#include <string>
#include "cxxopts.hpp"  // 需要包含头文件

int main(int argc, char* argv[]) {
    // 1. 创建 Options 对象（程序名和描述）
    cxxopts::Options options("argopt", "A simple example of cxxopts");

    // 2. 添加命令行选项
    options.add_options()
        // 无参选项（如 --help）
        ("h,help", "Print help")
        // 带参选项（值类型为 std::string）
        ("f", "option f", cxxopts::value<std::string>())
        // 带默认值的选项
        ("i", "option i", cxxopts::value<bool>()->default_value("false"))
        ("l", "option l", cxxopts::value<bool>()->default_value("false"))
        ("r", "option r", cxxopts::value<bool>()->default_value("false"))
        ("positional", "Positional arguments", cxxopts::value<std::vector<std::string>>());

    // 3. 解析命令行参数
    try {
        auto result = options.parse(argc, argv);

        // 4. 处理选项
        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }

        if (result.count("f")) {
            std::string file = result["f"].as<std::string>();
            std::cout << "filename: " << file << std::endl;
        }

        for (const auto& opt : std::vector<std::string>{"i", "l", "r"}) {
            if (result[opt].as<bool>()) {
                std::cout << "option: " << opt << std::endl;
            }
        }
    } catch (const cxxopts::exceptions::exception& e) {
        // 捕获解析错误（如缺少必需参数、类型不匹配）
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << options.help() << std::endl;
        return 1;
    }

    return 0;
}
