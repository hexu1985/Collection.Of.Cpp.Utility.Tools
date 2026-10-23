#include "socket.hpp"

#include <iostream>

using namespace unpsock;

int main(int argc, char* argv[])
{
	if(argc!=3){
        std::cerr << "Usage: " << argv[0] << " <IP> <port>\n";
		exit(1);
	}

    Socket sock(Family::INET, AddrType::DGRAM);

    auto serv_adr = Address::from_ip(argv[1], std::stoi(argv[2]));

    std::string message;
    const int BUF_SIZE = 30;
    while (true) {
        std::cout << "Input message(Q to quit): ";
        std::getline(std::cin, message);

        if (message == "q" || message == "Q") {
            break;
        }

        if (message.empty()) {
            continue;
        }

        message += '\n';
        sock.sendto(message, serv_adr);
        auto [recv_data, from_adr] = sock.recvfrom(BUF_SIZE);
        std::cout << "Message from server: " 
                  << std::string_view(reinterpret_cast<const char*>(recv_data.data()), recv_data.size()) 
                  << std::ends;
    }

    sock.close();

    return 0;
}
