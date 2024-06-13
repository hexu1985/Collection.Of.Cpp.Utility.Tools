#include "rational.hpp"

MemoryPool<Rational>* Rational::memPool = nullptr;

void* Rational::operator new(size_t size) { return memPool->alloc(size); }
void Rational::operator delete(void* doomed, size_t size) {
    memPool->free(doomed);
}
