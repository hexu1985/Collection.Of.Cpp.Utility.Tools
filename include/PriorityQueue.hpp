#ifndef MINI_UTILS_PRIORITY_QUEUE_INC
#define MINI_UTILS_PRIORITY_QUEUE_INC

#include <vector>
#include <stdexcept>

#include "heap_functions.hpp"

namespace MiniUtils {

// maintain a priority queue containing elements of data type
// T using a comparison function object of type Compare
template <typename T, typename Compare = std::greater<T> > 
class PriorityQueue {                       
public:
	PriorityQueue();
		// create empty priority queue
 
	int size() const;
		// return the number of elements in the priority queue
	bool isEmpty() const;
		// is the priority queue empty?

	void push(const T &item);
		// insert item into the priority queue
		// Postcondition: the heap size increases by 1
	void pop();
		// remove the element of highest priority.
		// Precondition: the priority queue is not empty. 
		// if condition fails, the function throws the
		// std::underflow_error exception.
		// Postcondition: the heap decreases by 1

	T &top();
		// return the element of highest priority
		// Precondition: the priority queue is not empty. 
		// if the condition fails, the function throws the
		// std::underflow_error exception
	const T &top() const;
		// constant version

private:
	std::vector<T> pqList_;
		// pqList_ holds the priority queue elements

	Compare comp_;
		// function object used for comparisons
};


// constructor. create empty priority queue
template <typename T, typename Compare>
PriorityQueue<T,Compare>::PriorityQueue()
{}


// return the size of the priority queue
template <typename T, typename Compare>
int PriorityQueue<T,Compare>::size() const
{ 
   return pqList_.size(); 
}

// return true if the priority queue is empty and false
// otherwise
template <typename T, typename Compare>
bool PriorityQueue<T,Compare>::isEmpty() const
{
   return pqList_.empty();
}

// insert a new item in the priority queue
template <typename T, typename Compare>
void PriorityQueue<T,Compare>::push(const T &item)
{
	// insert the item at the end of the vector
	// call push_heap() to restore the heap condition.
	pqList_.push_back(item);
	push_heap(pqList_, pqList_.size(), comp_);
}

// remove the element of highest priority,
template <typename T, typename Compare>
void PriorityQueue<T,Compare>::pop()
{
	// check for an empty priority queue
	if (pqList_.empty())
		throw std::underflow_error("PriorityQueue pop(): empty list");

	// call popHash() to put element at back of the vector
	pop_heap(pqList_, pqList_.size(), comp_);

	// delele element from back of pqList_
	pqList_.pop_back();
}

template <typename T, typename Compare>
T &PriorityQueue<T,Compare>::top()
{
   // check for an empty heap
   if (pqList_.empty())
		throw std::underflow_error("PriorityQueue top(): empty list");

	// return the root of the heap
	return pqList_[0];
}

template <typename T, typename Compare>
const T &PriorityQueue<T,Compare>::top() const
{
   // check for an empty heap
   if (pqList_.empty())
		throw std::underflow_error("PriorityQueue top(): empty list");

	// return the root of the heap
	return pqList_[0];
}

}   // namespace MiniUtils

#endif	// MINI_PRIORITY_QUEUE_CLASS
