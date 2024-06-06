#pragma once

#include <cstddef>

class NextOnFreeList {
public:
    NextOnFreeList* next;
};

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

inline
void* Rational::operator new(size_t size) {
    if (nullptr == freeList) {
        expandTheFreeList();
    }

    NextOnFreeList* head = freeList;
    freeList = head->next;

    return head;
}

inline
void Rational::operator delete(void* doomed, size_t size) {
    NextOnFreeList* head = static_cast<NextOnFreeList*>(doomed);

    head->next = freeList;
    freeList = head;
}

