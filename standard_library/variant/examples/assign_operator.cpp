#include <iomanip>
#include <iostream>
#include <string>
#include <type_traits>
#include <variant>
 
std::ostream& operator<<(std::ostream& os, std::variant<int, std::string> const& va)
{
    os << ": { ";
 
    std::visit([&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, int>)
            os << arg;
        else if constexpr (std::is_same_v<T, std::string>)
            os << std::quoted(arg);
    }, va);
 
    return os << " };\n";
}
 
int main()
{
    std::variant<int, std::string> a{2017}, b{"CppCon"};
    std::cout << "a" << a << "b" << b << '\n';
 
    std::cout << "(1) operator=( const variant& rhs )\n";
    a = b; 
    std::cout << "a" << a << "b" << b << '\n';
 
    std::cout << "(2) operator=( variant&& rhs )\n";
    a = std::move(b); 
    std::cout << "a" << a << "b" << b << '\n';
 
    std::cout << "(3) operator=( T&& t ), where T is int\n";
    a = 2019; 
    std::cout << "a" << a << '\n';
 
    std::cout << "(3) operator=( T&& t ), where T is std::string\n";
    std::string s{"CppNow"};
    std::cout << "s: " << std::quoted(s) << '\n';
    a = std::move(s);
    std::cout << "a" << a << "s: " << std::quoted(s) << '\n';
}
