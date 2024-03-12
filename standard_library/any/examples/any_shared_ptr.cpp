#include <iostream>
#include <memory>
#include <any>
 
int main()
{
    auto sp1 = std::make_shared<int>(5);
    std::cout << "in main(): sp1.use_count() == " << sp1.use_count()
              << " (object @ " << sp1 << ")\n";
 
    std::any any_ptr = sp1;
    std::cout << "after assign to any: sp1.use_count() == " << sp1.use_count()
              << " (object @ " << sp1 << ")\n";
}
