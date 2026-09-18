// demo_address_error.cpp
#include "socket.hpp"  // 替换为包含 AddressError 的头文件

#include <iostream>
#include <string>
#include <system_error>
#include <netdb.h>

using namespace unpsock;

// ============================================================
// 打印工具
// ============================================================
static void print_error(const std::string& title, const AddressError& err) {
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
// 演示各种常见 getaddrinfo 错误
// ============================================================
int main() {
    std::cout << "==== AddressError 常见错误演示 ====\n\n";

    // 1. 默认构造（gai_errnum = 0）
    {
        AddressError err("默认构造，无 getaddrinfo 错误码");
        print_error("默认构造 (gai_errnum = 0)", err);
    }

    // 2. 主机名无法解析
    {
        AddressError err("无法解析主机名: no-such-host.example", EAI_NONAME);
        print_error("EAI_NONAME - 主机名未知", err);
    }

    // 3. 临时性失败（可重试）
    {
        AddressError err("DNS 暂时不可用，请稍后重试", EAI_AGAIN);
        print_error("EAI_AGAIN - 临时失败", err);
    }

    // 4. 不可恢复的失败
    {
        AddressError err("名称解析发生不可恢复的错误", EAI_FAIL);
        print_error("EAI_FAIL - 不可恢复失败", err);
    }

    // 5. 内存不足
    {
        AddressError err("解析地址时内存分配失败", EAI_MEMORY);
        print_error("EAI_MEMORY - 内存不足", err);
    }

    // 6. 地址族不支持
    {
        AddressError err("不支持的地址族", EAI_FAMILY);
        print_error("EAI_FAMILY - 地址族不支持", err);
    }

    // 7. 服务名不支持
    {
        AddressError err("不支持的服务名", EAI_SERVICE);
        print_error("EAI_SERVICE - 服务名不支持", err);
    }

    // 8. socket 类型不支持
    {
        AddressError err("不支持的 socket 类型", EAI_SOCKTYPE);
        print_error("EAI_SOCKTYPE - socket 类型不支持", err);
    }

    // 9. 缓冲区太小
    {
        AddressError err("结果缓冲区太小", EAI_OVERFLOW);
        print_error("EAI_OVERFLOW - 缓冲区溢出", err);
    }

    // 10. 使用 std::error_code 重载构造
    {
        auto ec = make_gai_error_code(EAI_NONAME);
        AddressError err(ec, "通过 error_code 构造的地址错误");
        print_error("std::error_code 重载构造", err);
    }

    // 11. 直接对比 gai_strerror 输出，确认 errmsg() 内容一致
    {
        std::cout << "==== gai_strerror 原始输出对比 ====\n";
        const int codes[] = {
            EAI_NONAME, EAI_AGAIN, EAI_FAIL, EAI_MEMORY,
            EAI_FAMILY, EAI_SERVICE, EAI_SOCKTYPE, EAI_OVERFLOW
        };
        for (int c : codes) {
            AddressError err("compare", c);
            std::cout << "code=" << c
                      << "  gai_strerror=" << ::gai_strerror(c)
                      << "  err.errmsg()=" << err.errmsg()
                      << "\n";
        }
        std::cout << "\n";
    }

    // 12. 演示多态：用基类引用接收
    {
        std::cout << "==== 多态捕获演示 ====\n";
        try {
            throw AddressError("解析失败，抛出 AddressError", EAI_NONAME);
        } catch (const SocketException& e) {
            std::cout << "捕获为 SocketException\n";
            std::cout << "  errnum(): " << e.errnum() << "\n";
            std::cout << "  errmsg(): " << e.errmsg() << "\n";
            std::cout << "  what()  : " << e.what() << "\n";
        } catch (...) {
            std::cout << "捕获到未知异常\n";
        }
        std::cout << "\n";
    }

    // 13. 演示捕获为 std::system_error
    {
        std::cout << "==== 捕获为 std::system_error 演示 ====\n";
        try {
            throw AddressError("底层地址错误", EAI_FAIL);
        } catch (const std::system_error& e) {
            std::cout << "捕获为 std::system_error\n";
            std::cout << "  code().value()         : " << e.code().value() << "\n";
            std::cout << "  code().category().name(): "
                      << e.code().category().name() << "\n";
            std::cout << "  code().message()       : " << e.code().message() << "\n";
            std::cout << "  what()                 : " << e.what() << "\n";
        }
        std::cout << "\n";
    }

    std::cout << "==== 演示结束 ====\n";
    return 0;
}
