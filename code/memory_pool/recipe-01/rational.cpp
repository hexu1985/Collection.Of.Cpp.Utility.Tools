#include "rational.hpp"

class NextOnFreeList {
public:
    NextOnFreeList* next;
};

NextOnFreeList* Rational::freeList = nullptr;

void* Rational::operator new(size_t size) {
    if (nullptr == freeList) {
        expandTheFreeList();
    }

    NextOnFreeList* head = freeList;
    freeList = head->next;

    return head;
}

void Rational::operator delete(void* doomed, size_t size) {
    NextOnFreeList* head = static_cast<NextOnFreeList*>(doomed);

    head->next = freeList;
    freeList = head;
}

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
