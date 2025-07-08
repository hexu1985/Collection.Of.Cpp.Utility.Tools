#include <system_error>
#include <string>
#include <iostream>

// 1. 定义自己的错误枚举
enum class MyErrc {
    Success = 0,
    InvalidArgument,
    ResourceBusy,
    OperationTimeout,
    ConnectionFailed
};

// 2. 实现自定义的 error_category
class MyErrorCategory : public std::error_category {
public:
    // 返回类别名称
    const char* name() const noexcept override {
        return "MyErrorCategory";
    }
    
    // 返回错误描述
    std::string message(int ev) const override {
        switch (static_cast<MyErrc>(ev)) {
            case MyErrc::Success:           return "Success";
            case MyErrc::InvalidArgument:   return "Invalid argument";
            case MyErrc::ResourceBusy:      return "Resource busy";
            case MyErrc::OperationTimeout:  return "Operation timeout";
            case MyErrc::ConnectionFailed: return "Connection failed";
            default:                        return "Unknown error";
        }
    }
    
    // 定义错误代码到错误条件的映射
    std::error_condition default_error_condition(int ev) const noexcept override {
        switch (static_cast<MyErrc>(ev)) {
            case MyErrc::InvalidArgument:
                return std::errc::invalid_argument;
            case MyErrc::ResourceBusy:
                return std::errc::device_or_resource_busy;
            case MyErrc::OperationTimeout:
                return std::errc::timed_out;
            case MyErrc::ConnectionFailed:
                return std::errc::connection_refused;
            default:
                return std::error_condition(ev, *this);
        }
    }
};

// 3. 获取自定义类别的单例实例
const MyErrorCategory& my_error_category() {
    static MyErrorCategory instance;
    return instance;
}

// 4. 为 MyErrc 创建 error_code 和 error_condition 的转换函数
std::error_code make_error_code(MyErrc e) {
    return {static_cast<int>(e), my_error_category()};
}

std::error_condition make_error_condition(MyErrc e) {
    return {static_cast<int>(e), my_error_category()};
}

// 5. 注册 MyErrc 到 error_condition 系统
namespace std {
    template <>
    struct is_error_condition_enum<MyErrc> : true_type {};
}

// 示例函数可能抛出自定义错误
void doSomething(bool fail) {
    if (fail) {
        throw std::system_error(make_error_code(MyErrc::OperationTimeout));
    }
    std::cout << "Operation succeeded\n";
}

int main() {
    try {
        // 测试成功情况
        doSomething(false);
        
        // 测试失败情况
        doSomething(true);
    } catch (const std::system_error& e) {
        std::cout << "Caught system_error: " << e.what() << '\n';
        std::cout << "Error code: " << e.code() << " (" << e.code().message() << ")\n";
        
        // 检查错误条件
        if (e.code() == std::errc::timed_out) {
            std::cout << "This is a timeout condition\n";
        }
        
        // 直接比较自定义错误
        if (e.code() == MyErrc::OperationTimeout) {
            std::cout << "This is our OperationTimeout error\n";
        }
    }
    
    // 直接使用 error_code
    std::error_code ec = make_error_code(MyErrc::InvalidArgument);
    if (ec) {
        std::cout << "\nError code test: " << ec.message() << '\n';
        std::cout << "Category: " << ec.category().name() << '\n';
    }
    
    // 转换为 error_condition
    std::error_condition cond = ec.default_error_condition();
    std::cout << "Error condition: " << cond.message() << '\n';
    
    return 0;
}

