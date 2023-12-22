#include <iomanip>
#include <iostream>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>
 
// 要观览的变化体
using var_t = std::variant<int, long, double, std::string>;
 
// 观览器 #3 的辅助常量
template<class>
inline constexpr bool always_false_v = false;
 
// 观览器 #4 的辅助类型
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
// 显式推导指引（C++20 起不需要）
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;
 
int main()
{
    std::vector<var_t> vec = {10, 15l, 1.5, "hello"};
 
    for (auto&& v: vec)
    {
        // 1. void 观览器，仅为它的副作用而调用
        std::visit([](auto&& arg){ std::cout << arg; }, v);
 
        // 2. 返回值的观览器，返回另一 variant 的常见模式
        var_t w = std::visit([](auto&& arg) -> var_t { return arg + arg; }, v);
 
        // 3. 类型匹配观览器：以不同方式处理每个类型的 lambda
        std::cout << "。翻倍后，变化体持有";
        std::visit([](auto&& arg)
        {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int>)
                std::cout << "值为 " << arg << " 的 int\n";
            else if constexpr (std::is_same_v<T, long>)
                std::cout << "值为 " << arg << " 的 long\n";
            else if constexpr (std::is_same_v<T, double>)
                std::cout << "值为 " << arg << " 的 double\n";
            else if constexpr (std::is_same_v<T, std::string>)
                std::cout << "值为 " << std::quoted(arg) << " 的 std::string\n";
            else 
                static_assert(always_false_v<T>, "观览器无法穷尽类型！");
        }, w);
    }
 
    for (auto&& v: vec)
    {
        // 4. 另一种类型匹配观览器：有三个重载的 operator() 的类
        // 注：此情况下 '(auto arg)' 模板 operator() 将绑定到 'int' 与 'long'，
        //    但它不存在时 '(double arg)' operator() *也会* 绑定到 'int' 与 'long'，
        //    因为两者均可隐式转换到 double。使用此形式时应留心以正确处理隐式转换。
        std::visit(overloaded{
            [](auto arg) { std::cout << arg << ' '; },
            [](double arg) { std::cout << std::fixed << arg << ' '; },
            [](const std::string& arg) { std::cout << std::quoted(arg) << ' '; },
        }, v);
    }

    std::cout << std::endl;
}
