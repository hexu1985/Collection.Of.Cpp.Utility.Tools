#include <variant>
#include <string>
#include <iostream>
 
int main()
{
    std::variant<int, std::string> v1{2}, v2{"abc"}; 
    std::visit([] (auto&& x) { std::cout << x << ' '; }, v1);
    std::visit([] (auto&& x) { std::cout << x << '\n'; }, v2);
    v1.swap(v2);
    std::visit([] (auto&& x) { std::cout << x << ' '; }, v1);
    std::visit([] (auto&& x) { std::cout << x << '\n'; }, v2);
}
