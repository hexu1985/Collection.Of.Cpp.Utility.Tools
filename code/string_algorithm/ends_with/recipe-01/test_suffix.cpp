#include <iostream>
#include <string>
 
#include "ends_with.hpp"

void test_suffix_print(const std::string& str, const std::string& suffix)
{
    std::cout << '\'' << str << "' ends with '" << suffix << "': " <<
        ends_with(str, suffix) << '\n';
}
 
int main()
{
    std::boolalpha(std::cout);
    auto helloWorld = std::string("hello world");
 
    test_suffix_print(helloWorld, "world");
 
    test_suffix_print(helloWorld, "goodbye");
 
    test_suffix_print(helloWorld, "d");
 
    test_suffix_print(helloWorld, "x");
}
