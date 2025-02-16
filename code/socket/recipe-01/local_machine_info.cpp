#include <iostream>
#include "SocketUtility.hpp"

void print_machine_info() {
    auto host_name = Gethostname();
    auto ip_address = Gethostbyname(host_name);
    std::cout << "Host name: " << host_name << "\n";
    std::cout << "IP adddress: " << ip_address << "\n";
}

int main(int argc, char **argv)
{
    print_machine_info();
    return 0;
}
