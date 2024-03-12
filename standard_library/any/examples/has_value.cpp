#include <any>
#include <iostream>
#include <string>
 
int main()
{
    std::boolalpha(std::cout);
 
    std::any a0;
    std::cout << "a0.has_value(): " << a0.has_value() << '\n';
 
    std::any a1 = 42;
    std::cout << "a1.has_value(): " << a1.has_value() << '\n';
    std::cout << "a1 = " << std::any_cast<int>(a1) << '\n';
    a1.reset();
    std::cout << "a1.has_value(): " << a1.has_value() << '\n';
 
    auto a2 = std::make_any<std::string>("Milky Way");
    std::cout << "a2.has_value(): " << a2.has_value() << '\n';
    std::cout << "a2 = \"" << std::any_cast<std::string&>(a2) << "\"\n";
    a2.reset();
    std::cout << "a2.has_value(): " << a2.has_value() << '\n';
}
