#pragma once

#include "memory_pool.hpp"

class Rational {
public:
	Rational(int a = 0, int b = 1): n(a), d(b) {}

	void* operator new(size_t size); 
	void operator delete(void* doomed, size_t size); 

	static void newMemPool() { memPool = new MemoryPool<Rational>(64); }
	static void deleteMemPool() { delete memPool; }

private:
	static MemoryPool<Rational>* memPool;

private:
	int n;	// Numerator
	int d;	// Denominator
};

