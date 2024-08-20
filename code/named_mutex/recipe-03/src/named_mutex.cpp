#include "named_mutex.hpp"

NamedMutex::NamedMutex(const char* name): impl_(name) {
    InterprocessOnceFlag& once_flag = impl_.get().once_flag;
    InterprocessMutex* mutex = &impl_.get().mutex;
    interprocess_call_once(once_flag, [mutex]() { new (mutex) InterprocessMutex(); });
}

NamedMutex::~NamedMutex() {
}

void NamedMutex::lock() {
    InterprocessMutex& mutex = impl_.get().mutex;
    mutex.lock();
}

void NamedMutex::unlock() {
    InterprocessMutex& mutex = impl_.get().mutex;
    mutex.unlock();
}

bool NamedMutex::try_lock() {
    InterprocessMutex& mutex = impl_.get().mutex;
    return mutex.try_lock();
}

pthread_mutex_t* NamedMutex::native_handle() {
    InterprocessMutex& mutex = impl_.get().mutex;
    return mutex.native_handle();
}

bool NamedMutex::remove(const char* name) {
    return SharedMemoryObject::remove(name); 
}
