#ifndef DUMMY_LOCK_INC
#define DUMMY_LOCK_INC

class DummyLock {
public:
    inline void lock() {}
    inline void unlock() {}
};

#endif
