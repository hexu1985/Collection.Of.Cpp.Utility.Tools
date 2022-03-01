#include "rational.hpp"

NextOnFreeList *Rational::freeList = NULL;

void Rational::expandTheFreeList()
{
	// 我们必须分配足够大的对象以包含下一个指针
	size_t size = (sizeof(Rational) > sizeof(NextOnFreeList *)) ?
		sizeof(Rational) : sizeof(NextOnFreeList *);

	NextOnFreeList *runner = reinterpret_cast<NextOnFreeList *>(new char[size]);

	freeList = runner;
	for (int i = 0; i < EXPANSION_SIZE; i++) {
		runner->next =
			reinterpret_cast<NextOnFreeList *>(new char[size]);
		runner = runner->next;
	}
	runner->next = NULL;
}

void Rational::deleteMemPool()
{
    NextOnFreeList *nextPtr;
    for (nextPtr = freeList; nextPtr != NULL; nextPtr = freeList) {
        freeList = freeList->next;
        delete [] nextPtr;
    }
}
