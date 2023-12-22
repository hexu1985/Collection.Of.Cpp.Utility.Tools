#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <variant>
 
struct Demo {
    Demo(int) {}
    Demo(const Demo&) { throw std::domain_error("copy ctor"); }
    Demo& operator= (const Demo&) = default;
};
 
int main()
{
    std::variant<std::string, Demo> var{"str"};
    assert(var.index() == 0);
    assert(std::get<0>(var) == "str");
    assert(var.valueless_by_exception() == false);
 
    try {
        var = Demo{666};
    } catch (const std::domain_error& ex) {
        std::cout << "1) Exception: " << ex.what() << '\n';
    }
    assert(var.index() == std::variant_npos);
    assert(var.valueless_by_exception() == true);
 
    // 现在 var “无值”，此为 var 的初始化过程中引发的异常导致的非法状态。
 
    try {
        std::get<1>(var);
    } catch (const std::bad_variant_access& ex) {
        std::cout << "2) Exception: " << ex.what() << '\n';
    }
 
    var = "str2";
    assert(var.index() == 0);
    assert(std::get<0>(var) == "str2");
    assert(var.valueless_by_exception() == false);
}
