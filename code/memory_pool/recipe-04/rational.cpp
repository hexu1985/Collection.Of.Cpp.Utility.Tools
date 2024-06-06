#include "rational.hpp"

ChunkMemoryPool* Rational::memPool = nullptr;

void* Rational::operator new(size_t size) {
    return memPool->alloc(size); 
}

void Rational::operator delete(void* doomed, size_t size) {
    memPool->free(doomed);
}

void Rational::newMemPool() {
    memPool = new ChunkMemoryPool(sizeof(Rational), 64); 
}

void Rational::deleteMemPool() {
    delete memPool; 
}

