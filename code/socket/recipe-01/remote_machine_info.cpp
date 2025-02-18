#include <iostream>
#include "SocketUtility.hpp"
#include "SocketException.hpp"

void get_remote_machine_info() {
    std::string remote_host = "www.python.org";
    try {
        std::cout << "IP address of " << remote_host 
            << ": " << Gethostbyname(remote_host) << "\n";
    } catch (const HError& e) {
        std::cout << remote_host << ": " << e.what() << "\n";
    }
}

int main(int argc, char **argv)
{
    get_remote_machine_info();
    return 0;
}

