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
    auto message = sock.recv(30);

    std::cout << "Message from server: " << std::string(message.begin(), message.end()) << std::endl;

    sock.close();

    return 0;
}
