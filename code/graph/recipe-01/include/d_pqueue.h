#ifndef MINI_PRIORITY_QUEUE_CLASS
#define MINI_PRIORITY_QUEUE_CLASS

#include <vector>

#include "d_heap.h"
#include "d_except.h"

using namespace std;

// maintain a priority queue containing elements of data type
// T using a comparison function object of type Compare
template <typename T, typename Compare = greater<T> > 
class miniPQ
{                       
	public:
		miniPQ();
			// create empty priority queue
     
		int size() const;
			// return the number of elements in the priority queue
		bool empty() const;
			// is the priority queue empty?

		void push(const T& item);
			// insert item into the priority queue
			// Postcondition: the heap size increases by 1
		void pop();
			// remove the element of highest priority.
			// Precondition: the priority queue is not empty. 
			// if condition fails, the function throws the
			// underflowError exception.
			// Postcondition: the heap decreases by 1

		T& top();
			// return the element of highest priority
			// Precondition: the priority queue is not empty. 
			// if the condition fails, the function throws the
			// underflowError exception
		const T& top() const;
			// constant version

	private:
		vector<T> pqList;
			// pqList holds the priority queue elements

		Compare comp;
			// function object used for comparisons
};


// constructor. create empty priority queue
template <typename T, typename Compare>
miniPQ<T,Compare>::miniPQ()
{}


// return the size of the priority queue
template <typename T, typename Compare>
int miniPQ<T,Compare>::size() const
{ 
   return pqList.size(); 
}

// return true if the priority queue is empty and false
// otherwise
template <typename T, typename Compare>
bool miniPQ<T,Compare>::empty() const
{
   return pqList.empty();
}

// insert a new item in the priority queue
template <typename T, typename Compare>
void miniPQ<T,Compare>::push(const T& item)
{
	// insert the item at the end of the vector
	// call pushHeap() to restore the heap condition.
	pqList.push_back(item);
	pushHeap(pqList,pqList.size(), comp);
}

// remove the element of highest priority,
template <typename T, typename Compare>
void miniPQ<T,Compare>::pop()
{
	// check for an empty priority queue
	if (pqList.empty())
		throw underflowError("miniPQ pop(): empty list");

	// call popHash() to put element at back of the vector
	popHeap(pqList, pqList.size(), comp);

	// delele element from back of pqList
	pqList.pop_back();
}

template <typename T, typename Compare>
T& miniPQ<T,Compare>::top()
{
   // check for an empty heap
   if (pqList.empty())
		throw underflowError("miniPQ top(): empty list");

	// return the root of the heap
	return pqList[0];
}

template <typename T, typename Compare>
const T& miniPQ<T,Compare>::top() const
{
   // check for an empty heap
   if (pqList.empty())
		throw underflowError("miniPQ top(): empty list");

	// return the root of the heap
	return pqList[0];
}

#endif	// MINI_PRIORITY_QUEUE_CLASS
