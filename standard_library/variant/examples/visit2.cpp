#include <iostream>
#include <variant>

struct MultiplyVisitor
{
    float mFactor;
    
    MultiplyVisitor(float factor) : mFactor(factor) { }
    
    void operator()(int& i) const {
        i *= static_cast<int>(mFactor);
    }
    
    void operator()(float& f) const {
        f *= mFactor;
    }
    
    void operator()(std::string& ) const {
        // nothing to do here...
    }
};

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

int main() 
{
    auto PrintVisitor = [](const auto& t) { std::cout << t << "\n"; };
    auto TwiceMoreVisitor = [](auto& t) { t*= 2; };
    
    std::variant<int, float, std::string> intFloatString { "Hello" };
    std::visit(PrintVisitor, intFloatString);
    
    std::variant<int, float> intFloat { 20.4f };
    std::visit(PrintVisitor, intFloat);
    std::visit(TwiceMoreVisitor, intFloat);
    std::visit(PrintVisitor, intFloat);
    
    std::visit(MultiplyVisitor(0.5f), intFloat);
    std::visit(PrintVisitor, intFloat);
    
    std::variant<int, float, std::string> intFloatString2 { "Hello" };
    std::visit(overloaded{
        [](int& i) { i*= 2; },
        [](float& f) { f*= 2.0f; },
        [](std::string& s) { s = s + s; }
    }, intFloatString2);
    std::visit(PrintVisitor, intFloatString2);
    
    return 0;
}
