#define FMT_HEADER_ONLY

#include <iomanip>
#include <iostream>
#include "fmt/format.h"

int main() {
    std::cout << "Elapsed time: " 
        << std::setiosflags(std::ios::fixed) << std::setprecision(2) << 1.2345
        << " seconds\n"; 
    fmt::print("Elapsed time: {0:.2f} seconds\n", 1.2345);
}
