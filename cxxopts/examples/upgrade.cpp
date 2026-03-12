#include <iostream>
#include "cxxopts.hpp"

int main(int argc, char* argv[]) {
    try {
        // 创建 Options 对象，指定程序名称和简要描述
        cxxopts::Options options("MyApp", "A simple command-line application");

        // 添加命令行选项
        options.add_options()
            // 帮助选项，使用 -h 或 --help 触发，无参数
            ("h,help", "Print help")
            // 输入文件选项，使用 -i 或 --input 触发，需要一个字符串参数
            ("input", "Input file", cxxopts::value<std::string>())
            // 输出文件选项，使用 -o 或 --output 触发，需要一个字符串参数
            ("output", "Output file", cxxopts::value<std::string>())
            ("current_part", "System current part", cxxopts::value<std::string>())
            ("device_type", "device type", cxxopts::value<int>())
            ("sub_device_type", "Sub device type", cxxopts::value<int>());

        // 解析命令行参数
        auto result = options.parse(argc, argv);

        // 如果用户指定了 --help 或 -h 选项，打印帮助信息并退出程序
        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }

        // 检查是否指定了 --input 或 -i 选项，如果指定则输出输入文件名称
        if (result.count("input")) {
            std::cout << "Input file: " << result["input"].as<std::string>() << std::endl;
        } else {
            std::cerr << "错误：必须提供 --input 参数" << std::endl;
            std::cerr << "用法: " << argv[0] << " --input <文件名>" << std::endl;
            return 1;
        }

        // 输出输出文件名称，使用默认值或用户指定的值
        if (result.count("output")) {
            std::cout << "Output file: " << result["output"].as<std::string>() << std::endl;
        }

        if (result.count("current_part")) {
            std::cout << "Current part: " << result["current_part"].as<std::string>() << std::endl;
        }

        if (result.count("device_type")) {
            std::cout << "Device type: " << result["device_type"].as<int>() << std::endl;
        }

        if (result.count("sub_device_type")) {
            std::cout << "Sub device type: " << result["sub_device_type"].as<int>() << std::endl;
        }
    }
    catch (const cxxopts::exceptions::exception& e) {
        // 处理解析选项时可能出现的异常
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

