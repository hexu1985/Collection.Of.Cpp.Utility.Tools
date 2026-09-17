#include "socket.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

using namespace unpsock;

int main(int argc, char* argv[])
{
	if(argc!=3){
        std::cerr << "Usage: " << argv[0] << " <IP> <port>\n";
		exit(1);
	}

    Socket sock(Family::INET, AddrType::STREAM);
    auto serv_addr = Address::from_ip(argv[1], std::stoi(argv[2]));

    sock.connect(serv_addr);
    std::cout << "Connected..........." << std::endl;

    std::string message;
    const int BUF_SIZE = 1024;
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
        sock.sendall(message);
        auto data = sock.recv(BUF_SIZE-1);

        std::cout << "Message from server: " << std::string(data.begin(), data.end()) << std::ends;
    }

    sock.close();

    return 0;
}

