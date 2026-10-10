#include "cxxopts.hpp"
#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>

#include "socket.hpp"

using namespace unpsock;

void server(const std::string& host, int port) {
    Socket sock(Family::INET, AddrType::STREAM);
    sock.set_reuse_addr(true);
    sock.bind(Address::from_ip(host, port));
    sock.listen(1);
    std::cout << "Listening at " << sock.getsockname().to_string() << std::endl;

    while (true) {
        auto [sc, sockname] = sock.accept();
        std::cout << "Processing up to 1024 bytes at a time from "
                  << sockname.to_string() << std::endl;

        int n = 0;
        while (true) {
            auto data =sc.recv(1024);
            if (data.empty()) {
                break;
            }

            std::transform(data.begin(), data.end(), data.begin(),
                    [](uint8_t c) {
                    return static_cast<uint8_t>(std::toupper(static_cast<unsigned char>(c)));
                    });

            sc.sendall(data);
            n += data.size();

            std::printf("\r  %d bytes processed so far ", n);
            std::fflush(stdout);
        }

        std::printf("\n");
        sc.close();
        std::cout << "  Socket closed" << std::endl;
    }
}

void client(const std::string& host, int port, int bytecount) {
    Socket sock(Family::INET, AddrType::STREAM);
    bytecount = (bytecount + 15) / 16 * 16;
    std::string message = "capitalize this!";

    std::cout << "Sending " << bytecount
              << " bytes of data, in chunks of 16 bytes" << std::endl;
    sock.connect(Address::from_ip(host, port));

    int sent = 0;
    while (sent < bytecount) {
        sock.sendall(message);
        sent += message.size();
        std::printf("\r  %d bytes sent ", sent);
        std::fflush(stdout);
    }
    std::printf("\n");
    sock.shutdown_write();

    std::cout << "Receiving all the data the server sends back" << std::endl;

    int received = 0;
    while (true) {
        auto data = sock.recv(42);
        if (received == 0) {
            // 等价 Python repr(data) 打印首块数据
            std::cout << "  The first data received says \"";
            std::cout.write(reinterpret_cast<char *>(data.data()), data.size());
            std::cout << "\"" << std::endl;
        }
        if (data.empty()) {
            break;
        }
        received += data.size();
        std::printf("\r  %d bytes received ", received);
        std::fflush(stdout);
    }
    std::printf("\n");
    sock.close();
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
