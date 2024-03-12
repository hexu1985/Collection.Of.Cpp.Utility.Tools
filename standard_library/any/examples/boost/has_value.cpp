#include <boost/any.hpp>
#include <iostream>
#include <string>
 
int main()
{
    std::boolalpha(std::cout);
 
    boost::any a0;
    std::cout << "a0.has_value(): " << !a0.empty() << '\n';
 
    boost::any a1 = 42;
    std::cout << "a1.has_value(): " << !a1.empty() << '\n';
    std::cout << "a1 = " << boost::any_cast<int>(a1) << '\n';
    a1.clear();
    std::cout << "a1.has_value(): " << !a1.empty() << '\n';
 
    boost::any a2 = std::string("Milky Way");
    std::cout << "a2.has_value(): " << !a2.empty() << '\n';
    std::cout << "a2 = \"" << boost::any_cast<std::string&>(a2) << "\"\n";
    a2.clear();
    std::cout << "a2.has_value(): " << !a2.empty() << '\n';
}
