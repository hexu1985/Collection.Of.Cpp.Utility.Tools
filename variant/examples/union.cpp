#include <iostream>
#include <string>
#include <vector>
 
union S
{
    std::string str;
    std::vector<int> vec;
    ~S() { } // what to delete here?
};
 
int main()
{
    S s = {"Hello, world"};
    // at this point, reading from s.vec is undefined behavior
    std::cout << "s.str = " << s.str << '\n';
    
    // you have to call destructor of the contained objects!
    s.str.~basic_string<char>();
    
    // and a constructor!
    new (&s.vec) std::vector<int>;
    
    // now, s.vec is the active member of the union
    s.vec.push_back(10);
    std::cout << s.vec.size() << '\n';
    
    // anothe destructor
    s.vec.~vector<int>();
}
