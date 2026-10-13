#include "socket.hpp"

#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace unpsock;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "usage: unixbind <pathname>" << std::endl;
        return 1;
    }

    Socket sock(Family::UNIX, AddrType::STREAM);

    // 如果文件已存在则删除（失败也无所谓）
    unlink(argv[1]);

    auto addr = Address::from_unix(argv[1]);

    sock.bind(addr);

    // 获取绑定后的地址
    auto addr2 = sock.getsockname();

    // 打印路径和长度（与 Python 输出格式保持一致）
    std::cout << "bound name = " << addr2.to_string()
              << ", returned len = " << (addr2.to_string().length() + 1)
              << std::endl;

    sock.close();
    return 0;
}
