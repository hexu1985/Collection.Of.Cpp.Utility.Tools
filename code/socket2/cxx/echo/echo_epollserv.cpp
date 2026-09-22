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
    serv_sock.set_reuse_addr(true);   // 可选但推荐，避免重启时 EADDRINUSE

    auto serv_addr = Address::any(std::atoi(argv[1]), Family::INET);
    serv_sock.bind(serv_addr);
    serv_sock.listen(5);

    // ---- Selector ----
    EpollSelector sel;
    sel.init();
    sel.add(serv_sock, Event::Read);

    // key = fd，value = 客户端 Socket
    std::unordered_map<int, Socket> clients;

    const int BUF_SIZE = 100;
    while (true) {
        // 永久阻塞（原程序 epoll_wait(..., -1)）
        auto ready = sel.wait();   // timeout = nullopt

        for (const auto& ev : ready) {
            int fd = ev.fd;

            // 处理异常/挂断（epoll 总会报告）
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
                // ---- 新连接 ----
                auto [conn, peer] = serv_sock.accept();
                int conn_fd = conn.fileno();
                sel.add(conn, Event::Read);
                clients.emplace(conn_fd, std::move(conn));
                std::cout << "connected client: " << conn_fd << std::endl;
            } else {
                // ---- 客户端数据 ----
                auto it = clients.find(fd);
                if (it == clients.end()) continue;

                Socket& conn = it->second;
                auto data = conn.recv(BUF_SIZE);

                if (data.empty()) {
                    // 对端关闭
                    sel.remove(fd);
                    clients.erase(it);
                    std::cout << "closed client: " << fd << std::endl;
                } else {
                    conn.sendall(data);       // echo
                }
            }
        }
    }

    return 0;
}
