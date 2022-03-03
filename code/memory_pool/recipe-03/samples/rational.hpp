#ifndef RATIONAL_INC
#define RATIONAL_INC

#include "byte_memory_pool.hpp"

class Rational {
public:
	Rational(int a = 0, int b = 1): n(a), d(b) {}

	void *operator new(size_t size) { return memPool->alloc(size); }
	void operator delete(void *doomed, size_t size)
	{
		memPool->free(doomed);
	}

	static void newMemPool() { memPool = new mini_util::ByteMemoryPool; }
	static void deleteMemPool() { delete memPool; }

private:
	static mini_util::ByteMemoryPool *memPool;

private:
	int n;	// 分子
	int d;	// 分母
};

#endif
