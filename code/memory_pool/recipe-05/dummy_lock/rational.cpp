#include "rational.hpp"

MemoryPool<Rational>* Rational::stPool = nullptr;
MTMemoryPool<MemoryPool<Rational>, DummyLock>* Rational::memPool = nullptr;

void* Rational::operator new(size_t size) { return memPool->alloc(size); }
void Rational::operator delete(void* doomed, size_t size) {
    memPool->free(doomed);
}
