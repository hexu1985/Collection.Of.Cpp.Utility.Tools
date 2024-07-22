#include "interprocess_mutex.hpp"
 
int main(int argc, char* argv[]) {
    const char* name = "mtx";
    if (argc > 1) {
        name = argv[1];
    }
    
    InterprocessMutex::remove(name);
    return 0;
}
