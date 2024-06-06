#include "rational.hpp"

NextOnFreeList* Rational::freeList = nullptr;

void Rational::expandTheFreeList() {
	size_t size = (sizeof(Rational) > sizeof(NextOnFreeList*)) ?
		sizeof(Rational) : sizeof(NextOnFreeList*);

	NextOnFreeList* runner = reinterpret_cast<NextOnFreeList*>(new char[size]);

	freeList = runner;
	for (int i = 0; i < EXPANSION_SIZE; i++) {
		runner->next =
			reinterpret_cast<NextOnFreeList*>(new char[size]);
		runner = runner->next;
	}
	runner->next = nullptr;
}

void Rational::deleteMemPool() {
    NextOnFreeList* nextPtr;
    for (nextPtr = freeList; nextPtr != nullptr; nextPtr = freeList) {
        freeList = freeList->next;
        delete [] nextPtr;
    }
}
