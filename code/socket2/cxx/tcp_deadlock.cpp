#include "cxxopts.hpp"
#include <iostream>
#include <string>
#include <stdexcept>

void client(const std::string& host, int port, int bytecount) {
    // TODO: 实现 client 逻辑
    std::cout << "client host=" << host
              << " port=" << port
              << " bytecount=" << bytecount << std::endl;
}

void server(const std::string& host, int port) {
    // TODO: 实现 server 逻辑
    std::cout << "server host=" << host
              << " port=" << port << std::endl;
}

int main(int argc, char* argv[]) {
    try {
        cxxopts::Options options("deadlock_tcp",
                                 "Get deadlocked over TCP");

        options.add_options()
            ("role",
             "which role to play (client or server)",
             cxxopts::value<std::string>())
            ("host",
             "interface the server listens at; host the client sends to",
             cxxopts::value<std::string>())
            ("bytecount",
             "number of bytes for client to send (default 16)",
             cxxopts::value<int>()->default_value("16"))
            ("p",
             "TCP port (default 1060)",
             cxxopts::value<int>()->default_value("1060"))
            ("h,help", "Print usage");

        options.parse_positional({"role", "host", "bytecount"});
        options.positional_help("role host [bytecount]");

        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }

        // 检查必需的位置参数
        if (!result.count("role") || !result.count("host")) {
            std::cerr << "Error: missing required arguments 'role' and 'host'."
                      << std::endl;
            std::cout << options.help() << std::endl;
            return 1;
        }

        std::string role = result["role"].as<std::string>();
        std::string host = result["host"].as<std::string>();
        int bytecount = result["bytecount"].as<int>();
        int port = result["p"].as<int>();

        // 校验 role 的取值（对应 Python 中的 choices）
        if (role != "client" && role != "server") {
            std::cerr << "Error: argument role: invalid choice: '" << role
                      << "' (choose from 'client', 'server')" << std::endl;
            return 1;
        }

        if (role == "client") {
            client(host, port, bytecount);
        } else {
            server(host, port);
        }

    } catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
