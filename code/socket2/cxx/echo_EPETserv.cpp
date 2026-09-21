#include "socket.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace unpsock;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage : " << argv[0] << " <port>\n";
		exit(1);
    }

    // ---- 监听 socket ----
    Socket serv_sock(Family::INET, AddrType::STREAM);
    serv_sock.set_reuse_addr(true);

    auto serv_addr = Address::any(std::atoi(argv[1]), Family::INET);
    serv_sock.bind(serv_addr);
    serv_sock.listen(5);
    serv_sock.set_nonblocking(true);   // 原程序对监听 socket 也非阻塞

    EpollSelector sel;
    sel.init();
    // 监听 socket：LT（不带 EdgeTrigger），与原程序一致
    sel.add(serv_sock, Event::Read);

    std::unordered_map<int, Socket> clients;

    const int BUF_SIZE = 4;
    while (true) {
        auto ready = sel.wait();   // 永久阻塞
        std::cout << "return epoll_wait" << std::endl;

        for (const auto& ev : ready) {
            int fd = ev.fd;

            // 异常 / 挂断
            if (has_event(ev.events, Event::Error) ||
                    has_event(ev.events, Event::Hangup)) {
                if (fd != serv_sock.fileno()) {
                    sel.remove(fd);
                    clients.erase(fd);
                    std::cout << "closed client: " << fd << std::endl;
                }
                continue;
            }

            if (!has_event(ev.events, Event::Read)) continue;

            if (fd == serv_sock.fileno()) {
                // ---- 新连接（监听 socket 用 LT，accept 一次即可）----
                auto [conn, peer] = serv_sock.accept();
                int conn_fd = conn.fileno();

                // 客户端：非阻塞 + 边缘触发
                conn.set_nonblocking(true);
                sel.add(conn, Event::Read | Event::EdgeTrigger);   // ← 就这里不同

                clients.emplace(conn_fd, std::move(conn));
                std::cout << "connected client: " << conn_fd << std::endl;
            } else {
                // ---- 客户端数据：ET 必须循环读到 EAGAIN ----
                auto it = clients.find(fd);
                if (it == clients.end()) continue;

                Socket& conn = it->second;

                while (true) {
                    std::error_code ec;
                    auto data = conn.recv(BUF_SIZE, ec);

                    if (ec) {
                        if (ec == std::errc::resource_unavailable_try_again) {
                        } else {
                            std::cout << "recv fd=" << fd << ": " << ec.message() << std::endl;
                        }
                        break;
                    }
                    if (data.empty()) {              // 对端关闭
                        // 对端关闭
                        sel.remove(fd);
                        clients.erase(it);
                        std::cout << "closed client: " << fd << std::endl;
                        break;
                    }

                    conn.sendall(data);
                }
            }
        }
    }

    return 0;
}
