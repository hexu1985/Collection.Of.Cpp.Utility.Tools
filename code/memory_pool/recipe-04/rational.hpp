#pragma once

#include "chunk_memory_pool.hpp"

class Rational {
public:
	Rational(int a = 0, int b = 1): n(a), d(b) {}

	void* operator new(size_t size);
	void operator delete(void* doomed, size_t size);

	static void newMemPool();
	static void deleteMemPool();

private:
	static ChunkMemoryPool* memPool;

private:
	int n;	// Numerator
	int d;	// Denominator
};

