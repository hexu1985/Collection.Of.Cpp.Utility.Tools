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

    Socket serv_sock(Family::INET, AddrType::DGRAM);

    auto my_adr = Address::any(std::atoi(argv[1]));
    serv_sock.bind(my_adr);

    const int BUF_SIZE = 30;
    for (int i=0; i<3; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        auto [message, your_adr] = serv_sock.recvfrom(BUF_SIZE);
        serv_sock.sendto(message, your_adr);
        std::cout << "Message " << i+1 
                  << std::string_view(reinterpret_cast<const char*>(message.data()), message.size())
                  << std::endl;
    }

    serv_sock.close();

    return 0;
}

