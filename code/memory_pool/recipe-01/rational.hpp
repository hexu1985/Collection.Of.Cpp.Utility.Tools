#pragma once

#include <cstddef>

class NextOnFreeList;

class Rational {
public:
    enum { EXPANSION_SIZE = 32 };

	Rational(int a = 0, int b = 1): n(a), d(b) {}

	void* operator new(size_t size); 
	void operator delete(void* doomed, size_t size);

	static void newMemPool() { expandTheFreeList(); }
	static void deleteMemPool();

	static void expandTheFreeList();

private:
	static NextOnFreeList* freeList;

private:
	int n;	// Numerator
	int d;	// Denominator
};

