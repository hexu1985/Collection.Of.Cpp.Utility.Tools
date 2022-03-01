#ifndef RATIONAL_INC
#define RATIONAL_INC

#include <cstddef>

class NextOnFreeList {
public:
    NextOnFreeList *next;
};

class Rational {
public:
    enum { EXPANSION_SIZE = 32 };

	Rational(int a = 0, int b = 1): n(a), d(b) {}

	void *operator new(size_t size); 
	void operator delete(void *doomed, size_t size);

	static void newMemPool() { expandTheFreeList(); }
	static void deleteMemPool();

	static void expandTheFreeList();

private:
	static NextOnFreeList *freeList;

private:
	int n;	// 分子
	int d;	// 分母
};

inline
void *Rational::operator new(size_t size)
{
    if (NULL == freeList) {    // 如果列表为空，则将其填满
        expandTheFreeList();
    }

    NextOnFreeList *head = freeList;
    freeList = head->next;

    return head;
}

inline
void Rational::operator delete(void *doomed, size_t size)
{
    NextOnFreeList *head = static_cast<NextOnFreeList *>(doomed);

    head->next = freeList;
    freeList = head;
}

#endif
