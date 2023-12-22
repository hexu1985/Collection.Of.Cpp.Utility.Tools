#include <variant>
#include <string>
#include <iostream>

int main()
{
    std::variant<int, std::string> v = "abc";
 
    std::cout << "v.index = " << v.index() << '\n';
 
    v = {}; 
 
    std::cout << "v.index = " << v.index() << '\n';
}
