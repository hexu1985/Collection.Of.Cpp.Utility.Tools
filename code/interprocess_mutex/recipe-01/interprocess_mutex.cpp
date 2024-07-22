#include "interprocess_mutex.hpp"

InterprocessMutex::InterprocessMutex(const char* name): handle_(name) {
    pthread_mutex_t* mutex = &handle_.get().mutex;
}

InterprocessMutex::~InterprocessMutex() {
}

void InterprocessMutex::lock() {
}

void InterprocessMutex::unlock() {
}

bool InterprocessMutex::try_lock() {
    return true;
}

bool InterprocessMutex::remove(const char* name) {
    return true;
}
