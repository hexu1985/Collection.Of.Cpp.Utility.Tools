#include <iostream>
#include <variant>


int main() 
{
    auto PrintVisitor = [](auto& t) { std::cout << t << "\n"; };
    
    std::variant<int, float, std::string> intFloatString { "Hello" };
    std::visit(PrintVisitor, intFloatString);
    
    intFloatString = 10; // we're now an int
    std::visit(PrintVisitor, intFloatString);
    
    intFloatString.emplace<2>(std::string("Hello")); // we're now string again
    std::visit(PrintVisitor, intFloatString);
    
    // std::get returns a reference, so you can change the value:
    std::get<std::string>(intFloatString) += std::string(" World");
    std::visit(PrintVisitor, intFloatString);
    
    intFloatString = 10.1f;
    if (auto pFloat = std::get_if<float>(&intFloatString); pFloat)
        *pFloat *= 2.0f;
    
    std::visit(PrintVisitor, intFloatString);
    
    return 0;
}
