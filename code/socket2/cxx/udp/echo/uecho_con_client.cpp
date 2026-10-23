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
    sock.connect(serv_adr);

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
        sock.send(message);
        auto recv_data = sock.recv(BUF_SIZE);
        std::cout << "Message from server: " 
                  << std::string_view(reinterpret_cast<const char*>(recv_data.data()), recv_data.size()) 
                  << std::ends;
    }

    return 0;
}
