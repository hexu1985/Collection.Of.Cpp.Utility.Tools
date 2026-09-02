#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main ()
{
    milliseconds ms = milliseconds::zero();

    if ( ms == milliseconds::zero() )
        std::cout << "ms is zero.\n";
    else
        std::cout << ms.count() << ".\n";

  return 0;
}


