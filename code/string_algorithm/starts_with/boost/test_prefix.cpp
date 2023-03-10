#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>
 
using namespace boost::algorithm;
 
void test_prefix_print(const std::string& str, const std::string& prefix)
{
    std::cout << '\'' << str << "' starts with '" << prefix << "': " <<
        starts_with(str, prefix) << '\n';
}
 
int main()
{
    std::boolalpha(std::cout);
    auto helloWorld = std::string("hello world");
 
    test_prefix_print(helloWorld, "hello");
 
    test_prefix_print(helloWorld, "goodbye");
 
    test_prefix_print(helloWorld, "h");
 
    test_prefix_print(helloWorld, "x");
}
