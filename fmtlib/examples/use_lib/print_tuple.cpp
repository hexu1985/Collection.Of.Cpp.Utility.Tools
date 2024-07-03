#include <fmt/format.h>
#include <fmt/ranges.h>

using namespace std;
 
int main(int argc, char** argv)
{
    std::tuple<char, int, float> t{ 'a', 1, 2.0f };
    // Prints "('a', 1, 2.0)"
    fmt::print("{}\n", t);
 
 
    std::tuple<int, char> t2 = { 1, 'a' };
    // Prints "1, a"
    fmt::print("{}\n", fmt::join(t2, ", "));
 
 
    return 0;
}
