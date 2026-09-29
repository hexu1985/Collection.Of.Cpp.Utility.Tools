// tcp_deadlock.cpp
// TCP client and server that leave too much data waiting
// C++ 等价版本，故意保留死锁行为以作演示

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

// ---------------------------------------------------------------------------
// 工具：把 sockaddr 转成可读字符串（等价 Python 的 getsockname 打印）
// ---------------------------------------------------------------------------
static std::string sockaddr_to_string(const sockaddr_in& addr) {
    char ip[INET_ADDRSTRLEN] = {0};
    inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
    char buf[INET_ADDRSTRLEN + 16];
    std::snprintf(buf, sizeof(buf), "%s:%d", ip, ntohs(addr.sin_port));
    return std::string(buf);
}

// ---------------------------------------------------------------------------
// 服务器：读一块、回显一块（原样保留，因此大数据量时会死锁）
// ---------------------------------------------------------------------------
static void server(const std::string& host, uint16_t port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::perror("socket");
        std::exit(1);
    }

    int yes = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        std::perror("setsockopt");
        std::exit(1);
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) != 1) {
        std::cerr << "invalid host: " << host << "\n";
        std::exit(1);
    }

    if (bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        std::perror("bind");
        std::exit(1);
    }
    if (listen(sock, 1) < 0) {
        std::perror("listen");
        std::exit(1);
    }

    sockaddr_in bound{};
    socklen_t boundlen = sizeof(bound);
    getsockname(sock, reinterpret_cast<sockaddr*>(&bound), &boundlen);
    std::cout << "Listening at " << sockaddr_to_string(bound) << std::endl;

    while (true) {
        sockaddr_in peer{};
        socklen_t peerlen = sizeof(peer);
        int sc = accept(sock, reinterpret_cast<sockaddr*>(&peer), &peerlen);
        if (sc < 0) {
            std::perror("accept");
            continue;
        }

        std::cout << "Processing up to 1024 bytes at a time from "
                  << sockaddr_to_string(peer) << std::endl;

        long long n = 0;
        char buf[1024];
        while (true) {
            ssize_t len = recv(sc, buf, sizeof(buf), 0);
            if (len < 0) {
                std::perror("recv");
                break;
            }
            if (len == 0) break;  // 对端关闭写端

            // 原地大写转换（ASCII）
            for (ssize_t i = 0; i < len; ++i) {
                char c = buf[i];
                if (c >= 'a' && c <= 'z') buf[i] = static_cast<char>(c - 32);
            }

            // sendall 语义：循环直到全部发出
            ssize_t off = 0;
            while (off < len) {
                ssize_t sent = send(sc, buf + off, static_cast<size_t>(len - off), 0);
                if (sent < 0) {
                    std::perror("send");
                    break;
                }
                off += sent;
            }

            n += len;
            std::printf("\r  %lld bytes processed so far ", n);
            std::fflush(stdout);
        }

        std::printf("\n");
        close(sc);
        std::cout << "  Socket closed" << std::endl;
    }
    // 正常情况下不会到达
    close(sock);
}

// ---------------------------------------------------------------------------
// 客户端：发完后 shutdown(SHUT_WR)，再收全部回显（与 Python 版一致）
// ---------------------------------------------------------------------------
static void client(const std::string& host, uint16_t port, long long bytecount) {
    // 向上取整到 16 的倍数
    bytecount = (bytecount + 15) / 16 * 16;
    static const char message[] = "capitalize this!";  // 16 字节
    constexpr size_t msg_len = sizeof(message) - 1;    // 不含 '\0'

    std::cout << "Sending " << bytecount
              << " bytes of data, in chunks of 16 bytes" << std::endl;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::perror("socket");
        std::exit(1);
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) != 1) {
        std::cerr << "invalid host: " << host << "\n";
        std::exit(1);
    }
    if (connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        std::perror("connect");
        std::exit(1);
    }

    long long sent = 0;
    while (sent < bytecount) {
        ssize_t off = 0;
        while (off < static_cast<ssize_t>(msg_len)) {
            ssize_t s = send(sock, message + off,
                             static_cast<size_t>(msg_len) - static_cast<size_t>(off), 0);
            if (s < 0) {
                std::perror("send");
                std::exit(1);
            }
            off += s;
        }
        sent += static_cast<long long>(msg_len);
        std::printf("\r  %lld bytes sent ", sent);
        std::fflush(stdout);
    }
    std::printf("\n");

    if (shutdown(sock, SHUT_WR) < 0) {
        std::perror("shutdown");
    }

    std::cout << "Receiving all the data the server sends back" << std::endl;

    long long received = 0;
    char buf[42];  // 与 Python 版一致，42 字节
    while (true) {
        ssize_t len = recv(sock, buf, sizeof(buf), 0);
        if (len < 0) {
            std::perror("recv");
            break;
        }
        if (received == 0) {
            // 等价 Python repr(data) 打印首块数据
            std::cout << "  The first data received says \"";
            std::cout.write(buf, len);
            std::cout << "\"" << std::endl;
        }
        if (len == 0) break;
        received += len;
        std::printf("\r  %lld bytes received ", received);
        std::fflush(stdout);
    }
    std::printf("\n");
    close(sock);
}

// ---------------------------------------------------------------------------
// 入口：解析参数（role host [bytecount] [-p PORT]）
// ---------------------------------------------------------------------------
static void usage(const char* prog) {
    std::cerr << "usage: " << prog
              << " {client|server} host [bytecount] [-p PORT]\n";
    std::exit(2);
}

int main(int argc, char** argv) {
    std::string role;
    std::string host;
    long long bytecount = 16;
    uint16_t port = 1060;

    // 简化版参数解析，等价于 argparse 的常见用法
    std::string positional[3];
    int npos = 0;

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "-p") {
            if (i + 1 >= argc) usage(argv[0]);
            port = static_cast<uint16_t>(std::atoi(argv[++i]));
        } else if (a.rfind("-p", 0) == 0 && a.size() > 2) {
            port = static_cast<uint16_t>(std::atoi(a.c_str() + 2));
        } else {
            if (npos < 3) positional[npos++] = a;
        }
    }

    if (npos < 2) usage(argv[0]);
    role = positional[0];
    host = positional[1];
    if (npos >= 3) bytecount = std::atoll(positional[2].c_str());

    if (role != "client" && role != "server") usage(argv[0]);

    if (role == "client") {
        client(host, port, bytecount);
    } else {
        server(host, port);
    }
    return 0;
}
