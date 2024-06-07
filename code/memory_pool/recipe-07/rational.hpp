#pragma once

#include "memory_pool.hpp"

class Rational {
public:
    static constexpr size_t MAX_RATIONAL_COUNT = 5000;
    using MemoryPoolType = MemoryPool<Rational, MAX_RATIONAL_COUNT>;

	Rational(int a = 0, int b = 1): n(a), d(b) {}

	void *operator new(size_t size) { return memPool->alloc(size); }
	void operator delete(void* doomed, size_t size)
	{
		memPool->free(doomed);
	}

	static void newMemPool() { memPool = new MemoryPoolType(); }
	static void deleteMemPool() { delete memPool; }

private:
	static MemoryPoolType* memPool;

private:
	int n;	// Numerator
	int d;	// Denominator
};

