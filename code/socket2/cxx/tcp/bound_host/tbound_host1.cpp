#include "socket.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace unpsock;

int main(int argc, char* argv[])
{
    if(argc!=2){
        std::cerr << "Usage : " << argv[0] << " <port>\n";
        exit(1);
    }

    Socket serv_sock(Family::INET, AddrType::STREAM);

    auto serv_adr = Address::any(std::atoi(argv[1]));
    serv_sock.bind(serv_adr);
    serv_sock.listen(5);

    const int BUF_SIZE = 30;
    auto [clnt_sock, clnt_adr] = serv_sock.accept();
    std::cout << "Connected client " << std::endl;

    int i = 0;
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        auto data = clnt_sock.recv(BUF_SIZE);
        if (data.empty()) {
            break;
        }
        auto message = std::string_view(reinterpret_cast<const char*>(data.data()), data.size());
        std::cout << "Message " << i + 1 << ": " << message << std::endl;
        i += 1;
    }
    clnt_sock.close();

    serv_sock.close();

    return 0;
}

