#ifndef HEAP_FUNCTIONS
#define HEAP_FUNCTIONS

#include <vector>
#include <functional>

using namespace std;

// the vector elements in the range [0, last-1) are a heap.
// insert the element v[last-1] into the heap so that the
// range [0, last) is a heap. use the function object comp
// to perform comparisons
template <typename T, typename Compare>
void pushHeap(vector<T>& v, int last, Compare comp);

// filter the vector element v[first] down the heap with index
// range [first, last)
template <typename T, typename Compare>
void adjustHeap(vector<T>& v, int first, int last, Compare comp);

// the vector elements in the range [0, last) are a heap.
// swap the first and last elements of the heap and then
// make the elements in the index range [0, last-1) a heap.
// use the function object comp to perform comparisons
template <typename T, typename Compare>
void popHeap(vector<T>& v, int last, Compare comp);

// arrange the vector elements into a heap. use the function
// object comp to perform comparisons
template <typename T, typename Compare>
void makeHeap(vector<T>& v, Compare comp);

// implementations

template <typename T, typename Compare>
void pushHeap(vector<T>& v, int last, Compare comp)
{
	// assume the new item is at location v[last-1] and that
	// the elements v[0] to v[last-2] are in heap order
	int currentPos, parentPos;
	T target;

	// currentPos is an index that traverses path of parents.
	// target is value hlist[i] and is repositioned in path
	currentPos = last-1;
	parentPos = (currentPos-1)/2;
	target = v[last-1];

	// traverse path of parents up to the root
	while (currentPos != 0)
	{
		// compare target and parent value
		if (comp(target,v[parentPos]))
		{
			// move data from parent position to current
			// position. update current position to parent
			// position. compute next parent
			v[currentPos] = v[parentPos];
			currentPos = parentPos;
			parentPos = (currentPos-1)/2;
		}
		else
			// if !comp(target, parentvalue), heap condition is ok. break
			break;
	}
	// the correct location has been discovered. assign target
	v[currentPos] = target;
}


template <typename T, typename Compare>
void adjustHeap(vector<T>& v, int first, int last, Compare comp)
{
	int currentPos, childPos;
	T target;

	// start at first and filter target down the heap
	currentPos = first;
	target = v[first];

	// compute the left child index and begin a scan down
	// path of children, stopping at end of list (last)
	// or when we find a place for target
	childPos = 2 * currentPos + 1;
	while (childPos <= last-1)
	{
		// index of right child is childPos+1. compare the
		// two children. change childPos if
		// comp(v[childPos+1], v[childPos]) is true
		if ((childPos+1 <= last-1) &&
            comp(v[childPos+1], v[childPos]))
			childPos = childPos + 1;

		// compare selected child to target
		if (comp(v[childPos],target))
		{
			// comp(selected child, target) is true.
			// move selected child to the parent;
			// position of selected child is now vacated
			v[currentPos] = v[childPos];

			// update indices to continue the scan
			currentPos = childPos;
			childPos = 2 * currentPos + 1;
		}
		else
			// target belongs at currentPos
			break;
	}
	v[currentPos] = target;
}

template <typename T, typename Compare>
void popHeap(vector<T>& v, int last, Compare comp)
{
	T temp;

	// exchange the first and last element in the heap
	temp = v[0];
	v[0] = v[last-1];
	v[last-1] = temp;

	// filter down the root over the range [0, last-1)
	adjustHeap(v, 0, last-1, comp);
}

template <typename T, typename Compare>
void makeHeap(vector<T>& v, Compare comp)
{
	int heapPos, lastPos;

	// compute the size of teh heap and the index
	// of the last parent
	lastPos = v.size();
	heapPos = (lastPos - 2)/2;

	// filter down every parent in order from last parent
	// down to root
	while (heapPos >= 0)
	{
		adjustHeap(v,heapPos, lastPos, comp);
		heapPos--;
	}
}

#endif	// HEAP_FUNCTIONS
