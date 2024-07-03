#include <cassert>

#define FMT_HEADER_ONLY
#include "fmt/format.h"
 
int main()
{
    std::string message = fmt::format("The answer is {}.", 42);
    assert(message == "The answer is 42.");
}
