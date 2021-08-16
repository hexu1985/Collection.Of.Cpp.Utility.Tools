#ifndef RATIONAL_INC
#define RATIONAL_INC

#include "DummyLock.hpp"
#include "MemoryPool.hpp"
#include "MTMemoryPool.hpp"

class Rational {
public:
	Rational(int a = 0, int b = 1): n(a), d(b) {}

	void *operator new(size_t size) { return memPool->alloc(size); }
	void operator delete(void *doomed, size_t size)
	{
		memPool->free(doomed);
	}

	static void newMemPool() { memPool = new mini_util::MTMemoryPool<mini_util::MemoryPool<Rational>, DummyLock>; }
	static void deleteMemPool() { delete memPool; }

private:
	static mini_util::MTMemoryPool<mini_util::MemoryPool<Rational>, DummyLock> *memPool;

private:
	int n;	// 分子
	int d;	// 分母
};

#endif
