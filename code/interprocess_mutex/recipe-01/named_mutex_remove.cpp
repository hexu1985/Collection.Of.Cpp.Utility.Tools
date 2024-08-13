#include "shared_memory_object.hpp"
 
int main(int argc, char* argv[]) {
    const char* name = "mtx";
    if (argc > 1) {
        name = argv[1];
    }
    
    SharedMemoryObject::remove(name);
    return 0;
}
