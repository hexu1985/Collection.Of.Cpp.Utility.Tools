#include <cassert>
#include <iostream>
#include <string>
#include <variant>
#include <vector>
 
template <class Os> Os& operator<< (Os& os, const std::vector<int>& v) {
    os << "{ ";
    for (int e: v) { std::cout << e << ' '; }
    return os << "}";
}
 
int main()
{
    {
        std::variant<int, std::string> var; // 值初始化第一个可选项
        assert(std::holds_alternative<int>(var) && var.index() == 0 &&
               std::get<int>(var) == 0);
    }
    {
        std::variant<std::string, int> var{"STR"};
            // 用 std::string{"STR"}; 初始化第一个可选项
        std::cout << "1) " << std::get<std::string>(var) << '\n';
    }
    {
        std::variant<std::string, int> var{42};
            // 用 int = 42; 初始化第二个可选项
        assert(std::holds_alternative<int>(var));
        std::cout << "2) " << std::get<int>(var) << '\n';
    }
    {
        std::variant<std::string, std::vector<int>, float> var{
            std::in_place_type<std::string>, 4, 'A'};
            // 用 std::string{4, 'A'}; 初始化第一个可选项
        assert(var.index() == 0);
        std::cout << "3) " << std::get<std::string>(var) << '\n';
    }
    {
        std::variant<std::string, std::vector<int>, char> var{
            std::in_place_type<std::vector<int>>, {1,2,3,4,5} };
            // 用 std::vector{1,2,3,4,5}; 初始化第二个可选项
        assert(var.index() == 1);
        std::cout << "4) " << std::get<std::vector<int>>(var) << '\n'; 
    }
    {
        std::variant<std::string, std::vector<int>, bool> var{
            std::in_place_index<0>, "ABCDE", 3};
            // 用 std::string{"ABCDE", 3}; 初始化第一个可选项
        assert(var.index() == 0);
        std::cout << "5) " << std::get<std::string>(var) << '\n';
    }
    {
        std::variant<std::string, std::vector<int>, char> var{
            std::in_place_index<1>, 4, 42};
            // 用 std::vector(4, 42); 初始化第二个可选项
        assert(std::holds_alternative<std::vector<int>>(var));
        std::cout << "6) " << std::get<std::vector<int>>(var) << '\n';
    }
}
