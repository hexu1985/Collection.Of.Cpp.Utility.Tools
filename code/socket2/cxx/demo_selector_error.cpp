// demo_selector_error.cpp
#include "socket.hpp"  // 替换为包含 SelectorError 的头文件

#include <iostream>
#include <string>
#include <system_error>
#include <cerrno>
#include <cstring>

using namespace unpsock;

// ============================================================
// 打印工具
// ============================================================
static void print_error(const std::string& title, const SelectorError& err) {
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
// 演示各种常见 Selector 错误
// ============================================================
int main() {
    std::cout << "==== SelectorError 常见错误演示 ====\n\n";

    // ---------------- 自定义 SelectorErrc 错误 ----------------

    // 1. 默认构造（errnum = 0，走 system_category）
    {
        SelectorError err("默认构造，无错误码");
        print_error("默认构造 (errnum = 0)", err);
    }

    // 2. Success —— 一般不会作为异常抛出，但演示 category 输出
    {
        auto ec = make_selector_ec(SelectorErrc::Success);
        SelectorError err(ec, "（仅演示）Success 状态");
        print_error("SelectorErrc::Success", err);
    }

    // 3. fd 已注册
    {
        auto ec = make_selector_ec(SelectorErrc::AlreadyRegistered);
        SelectorError err(ec, "fd=7 已经注册到 selector，无法重复 add");
        print_error("SelectorErrc::AlreadyRegistered", err);
    }

    // 4. fd 未注册
    {
        auto ec = make_selector_ec(SelectorErrc::NotRegistered);
        SelectorError err(ec, "fd=7 尚未注册，无法 modify/remove");
        print_error("SelectorErrc::NotRegistered", err);
    }

    // 5. 找不到可用后端
    {
        auto ec = make_selector_ec(SelectorErrc::BackendUnavailable);
        SelectorError err(ec, "epoll/kqueue/poll 均不可用，无法创建 selector");
        print_error("SelectorErrc::BackendUnavailable", err);
    }

    // 6. 底层 wait 失败
    {
        auto ec = make_selector_ec(SelectorErrc::WaitFailed);
        SelectorError err(ec, "selector wait 返回错误");
        print_error("SelectorErrc::WaitFailed", err);
    }

    // ---------------- 底层 errno 错误（走 system_category）----------------

    // 7. wait 被信号中断
    {
        SelectorError err("selector wait 被信号中断", EINTR);
        print_error("EINTR - wait 被信号中断", err);
    }

    // 8. 无效参数
    {
        SelectorError err("selector 参数无效", EINVAL);
        print_error("EINVAL - 参数无效", err);
    }

    // 9. 无效文件描述符
    {
        SelectorError err("selector 使用了无效的 fd", EBADF);
        print_error("EBADF - 无效 fd", err);
    }

    // 10. 内存不足
    {
        SelectorError err("selector 内部内存分配失败", ENOMEM);
        print_error("ENOMEM - 内存不足", err);
    }

    // 11. 资源暂时不可用
    {
        SelectorError err("selector 资源暂时不可用，请重试", EAGAIN);
        print_error("EAGAIN - 资源暂时不可用", err);
    }

    // ---------------- std::error_code 重载构造 ----------------

    // 12. 使用 SelectorErrc 的 error_code 构造
    {
        auto ec = make_selector_ec(SelectorErrc::AlreadyRegistered);
        SelectorError err(ec, "通过 error_code 构造的 Selector 错误");
        print_error("std::error_code 重载构造 (SelectorErrc)", err);
    }

    // 13. 使用系统 errno 的 error_code 构造
    {
        std::error_code ec(ECONNREFUSED, std::system_category());
        SelectorError err(ec, "通过 error_code 构造的 Selector 系统错误");
        print_error("std::error_code 重载构造 (system)", err);
    }

    // ---------------- 原始输出对比 ----------------

    // 14. 与 selector_category().message() 对比
    {
        std::cout << "==== selector_category 原始 message 对比 ====\n";
        const SelectorErrc codes[] = {
            SelectorErrc::Success,
            SelectorErrc::AlreadyRegistered,
            SelectorErrc::NotRegistered,
            SelectorErrc::BackendUnavailable,
            SelectorErrc::WaitFailed,
        };
        for (auto c : codes) {
            auto ec = make_selector_ec(c);
            SelectorError err(ec, "compare");
            std::cout << "enum=" << static_cast<int>(c)
                      << "  category.message=" << ec.message()
                      << "  err.errmsg()=" << err.errmsg()
                      << "\n";
        }
        std::cout << "\n";
    }

    // 15. 与 strerror 原始输出对比（系统错误部分）
    {
        std::cout << "==== strerror 原始输出对比 ====\n";
        const int codes[] = { EINTR, EINVAL, EBADF, ENOMEM, EAGAIN };
        for (int c : codes) {
            SelectorError err("compare", c);
            std::cout << "errno=" << c
                      << "  strerror=" << std::strerror(c)
                      << "  err.errmsg()=" << err.errmsg()
                      << "\n";
        }
        std::cout << "\n";
    }

    // ---------------- 多态捕获演示 ----------------

    // 16. 捕获为 SocketException
    {
        std::cout << "==== 捕获为 SocketException 演示 ====\n";
        try {
            throw SelectorError(
                make_selector_ec(SelectorErrc::NotRegistered),
                "fd 未注册");
        } catch (const SocketException& e) {
            std::cout << "捕获为 SocketException\n";
            std::cout << "  errnum(): " << e.errnum() << "\n";
            std::cout << "  errmsg(): " << e.errmsg() << "\n";
            std::cout << "  what()  : " << e.what() << "\n";
        }
        std::cout << "\n";
    }

    // 17. 捕获为 std::system_error
    {
        std::cout << "==== 捕获为 std::system_error 演示 ====\n";
        try {
            throw SelectorError(
                make_selector_ec(SelectorErrc::BackendUnavailable),
                "没有可用的 selector 后端");
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

    std::cout << "==== 演示结束 ====\n";
    return 0;
}
