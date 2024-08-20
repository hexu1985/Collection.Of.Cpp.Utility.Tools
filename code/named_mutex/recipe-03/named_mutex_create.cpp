#include "named_mutex.hpp"
 
int main(int argc, char* argv[]) {
    const char* name = "mtx";
    if (argc > 1) {
        name = argv[1];
    }
    
    NamedMutex global_mutex(name);
    return 0;
}
