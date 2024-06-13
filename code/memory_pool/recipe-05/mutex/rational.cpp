#include "rational.hpp"

MemoryPool<Rational>* Rational::stPool = 0;
MTMemoryPool<MemoryPool<Rational>, std::mutex>* Rational::memPool = 0;

void* Rational::operator new(size_t size) { return memPool->alloc(size); }
void Rational::operator delete(void* doomed, size_t size) {
    memPool->free(doomed);
}
