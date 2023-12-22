#include <iostream>
#include <variant>
#include <string>


int main() 
{
std::cout << "sizeof string: " 
          << sizeof(std::string) << "\n";
          
std::cout << "sizeof variant<int, string>: " 
          << sizeof(std::variant<int, std::string>) << "\n";
          
std::cout << "sizeof variant<int, float>: " 
          << sizeof(std::variant<int, float>) << "\n";
          
std::cout << "sizeof variant<int, double>: " 
          << sizeof(std::variant<int, double>) << "\n";
}
