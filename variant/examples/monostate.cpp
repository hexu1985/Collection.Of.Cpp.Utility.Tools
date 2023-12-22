#include <variant>
#include <iostream>
 
struct S
{
    S(int i) : i(i) {}
    int i;
};
 
int main() {
 
    // 若无 monostate 类型则此声明将失败。
    // 这是因为 S 不可默认构造。
 
    std::variant<std::monostate, S> var; 
 
    // var.index() 现为 0 ——首个元素
    // std::get<S> 将抛异常！我们需要先赋一个值
 
    var = 12; 
 
    std::cout << std::get<S>(var).i << '\n';
}
