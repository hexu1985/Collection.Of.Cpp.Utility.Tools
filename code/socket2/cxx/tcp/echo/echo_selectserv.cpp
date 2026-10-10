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

    Socket serv_sock(Family::INET, AddrType::STREAM);

    auto serv_addr = Address::any(std::atoi(argv[1]), Family::INET);
    serv_sock.bind(serv_addr);
    serv_sock.listen(5);

    // ---- Selector ----
    SelectSelector sel;
    sel.add(serv_sock, Event::Read);

    // 保存所有已连接客户端；key = fd
    std::unordered_map<int, Socket> clients;

    const int BUF_SIZE = 100;
    while (true) {
        auto ready = sel.wait(std::chrono::milliseconds(5005));
        if (ready.empty()) continue;   // 超时

        for (const auto& ev : ready) {
            int fd = ev.fd;

            if (has_event(ev.events, Event::Read)) {
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
                    if (it == clients.end()) continue;   // 已被清理

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
    }

    return 0;
}
