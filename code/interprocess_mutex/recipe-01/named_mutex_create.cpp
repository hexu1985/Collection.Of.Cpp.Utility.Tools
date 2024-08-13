#include <new>
#include "interprocess_mutex.hpp"
#include "shared_memory.hpp"
 
int main(int argc, char* argv[]) {
    const char* name = "mtx";
    if (argc > 1) {
        name = argv[1];
    }
    
    SharedMemory<InterprocessMutex> global_mutex(name);
    InterprocessMutex* ptr = &global_mutex.get();
    new (ptr) InterprocessMutex;    // replacement new
    return 0;
}
