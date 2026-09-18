// demo_socket_error.cpp
#include "socket.hpp"  // 替换为包含 SocketError 的头文件

#include <iostream>
#include <string>
#include <system_error>
#include <cerrno>
#include <cstring>

using namespace unpsock;

// ============================================================
// 打印工具
// ============================================================
static void print_error(const std::string& title, const SocketError& err) {
    std::cout << "----------------------------------------\n";
    std::cout << "场景    : " << title << "\n";
    std::cout << "what()  : " << err.what() << "\n";
    std::cout << "errnum(): " << err.errnum() << "\n";
    std::cout << "errmsg(): " << err.errmsg() << "\n";
    std::cout << "category: " << err.code().category().name() << "\n";
    std::cout << "code    : " << err.code().value() << "\n";
    std::cout << "----------------------------------------\n\n";
}

// ============================================================
// 演示各种常见 socket / errno 错误
// ============================================================
int main() {
    std::cout << "==== SocketError 常见错误演示 ====\n\n";

    // 1. 默认构造（errnum = 0）
    {
        SocketError err("默认构造，无系统错误码");
        print_error("默认构造 (errnum = 0)", err);
    }

    // 2. 权限不足
    {
        SocketError err("绑定端口 80 需要 root 权限", EACCES);
        print_error("EACCES - 权限不足", err);
    }

    // 3. 地址已被占用
    {
        SocketError err("端口已被占用，无法绑定", EADDRINUSE);
        print_error("EADDRINUSE - 地址已被占用", err);
    }

    // 4. 地址不可用
    {
        SocketError err("请求的地址在当前主机上不可用", EADDRNOTAVAIL);
        print_error("EADDRNOTAVAIL - 地址不可用", err);
    }

    // 5. 连接被拒绝
    {
        SocketError err("目标服务器拒绝连接", ECONNREFUSED);
        print_error("ECONNREFUSED - 连接被拒绝", err);
    }

    // 6. 连接被对端重置
    {
        SocketError err("连接被对端重置", ECONNRESET);
        print_error("ECONNRESET - 连接被重置", err);
    }

    // 7. 连接超时
    {
        SocketError err("连接超时", ETIMEDOUT);
        print_error("ETIMEDOUT - 连接超时", err);
    }

    // 8. 网络不可达
    {
        SocketError err("网络不可达", ENETUNREACH);
        print_error("ENETUNREACH - 网络不可达", err);
    }

    // 9. 主机不可达
    {
        SocketError err("主机不可达", EHOSTUNREACH);
        print_error("EHOSTUNREACH - 主机不可达", err);
    }

    // 10. 资源暂时不可用（非阻塞操作常见）
    {
        SocketError err("当前资源暂时不可用，请重试", EAGAIN);
        print_error("EAGAIN - 资源暂时不可用", err);
    }

    // 11. 被信号中断
    {
        SocketError err("系统调用被信号中断", EINTR);
        print_error("EINTR - 系统调用被中断", err);
    }

    // 12. 无效的文件描述符
    {
        SocketError err("使用了无效的 socket 描述符", EBADF);
        print_error("EBADF - 无效的文件描述符", err);
    }

    // 13. 参数无效
    {
        SocketError err("传入了无效的参数", EINVAL);
        print_error("EINVAL - 无效参数", err);
    }

    // 14. 对端已关闭连接
    {
        SocketError err("对端已关闭写端", EPIPE);
        print_error("EPIPE - 管道破裂/对端关闭", err);
    }

    // 15. 使用 std::error_code 重载构造
    {
        std::error_code ec(ECONNREFUSED, std::system_category());
        SocketError err(ec, "通过 error_code 构造的 socket 错误");
        print_error("std::error_code 重载构造", err);
    }

    // 16. 与 strerror 原始输出对比
    {
        std::cout << "==== strerror 原始输出对比 ====\n";
        const int codes[] = {
            EACCES, EADDRINUSE, EADDRNOTAVAIL, ECONNREFUSED,
            ECONNRESET, ETIMEDOUT, ENETUNREACH, EHOSTUNREACH,
            EAGAIN, EINTR, EBADF, EINVAL, EPIPE
        };
        for (int c : codes) {
            SocketError err("compare", c);
            std::cout << "errno=" << c
                      << "  strerror=" << std::strerror(c)
                      << "  err.errmsg()=" << err.errmsg()
                      << "\n";
        }
        std::cout << "\n";
    }

    // 17. 多态：捕获为 SocketException
    {
        std::cout << "==== 捕获为 SocketException 演示 ====\n";
        try {
            throw SocketError("连接被拒绝", ECONNREFUSED);
        } catch (const SocketException& e) {
            std::cout << "捕获为 SocketException\n";
            std::cout << "  errnum(): " << e.errnum() << "\n";
            std::cout << "  errmsg(): " << e.errmsg() << "\n";
            std::cout << "  what()  : " << e.what() << "\n";
        }
        std::cout << "\n";
    }

    // 18. 多态：捕获为 std::system_error
    {
        std::cout << "==== 捕获为 std::system_error 演示 ====\n";
        try {
            throw SocketError("底层 socket 错误", ETIMEDOUT);
        } catch (const std::system_error& e) {
            std::cout << "捕获为 std::system_error\n";
            std::cout << "  code().value()          : " << e.code().value() << "\n";
            std::cout << "  code().category().name(): "
                      << e.code().category().name() << "\n";
            std::cout << "  code().message()        : " << e.code().message() << "\n";
            std::cout << "  what()                  : " << e.what() << "\n";
        }
        std::cout << "\n";
    }

    // 19. 模拟真实 socket 调用失败（示例：socket() 传非法族）
    {
        std::cout << "==== 真实 socket 调用失败演示 ====\n";
        int fd = ::socket(-1, SOCK_STREAM, 0);
        if (fd < 0) {
            SocketError err("socket() 调用失败", errno);
            print_error("真实 socket() 调用返回 -1", err);
        } else {
            std::cout << "socket() 意外成功, fd=" << fd << "\n\n";
            ::close(fd);
        }
    }

    std::cout << "==== 演示结束 ====\n";
    return 0;
}
