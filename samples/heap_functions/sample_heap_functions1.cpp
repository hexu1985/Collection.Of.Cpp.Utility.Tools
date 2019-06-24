// program illustrates push_heap()/pop_heap() for vectors vA and vB;
// vA is array-based tree for maximum heap and vB for minimum heap.
// loop copies 7 elements from arr to vA and vB using push_heap()
// using objects greater<int>() and less<int>() respectively.
// calls to pop_heap() remove elements from two heaps creating
// output in ascending and descending order

#include <iostream>
#include <vector>

#include "heap_functions.hpp"

using namespace std;
using namespace MiniUtils;

int main()
{
	int arr[] = {5, 9, 2, 7, 1, 3, 8};
	int i, arrSize = sizeof(arr)/sizeof(int);
	vector<int> vA, vB;

	// load arr element in vA as a maximum heap and
	// in vB as a minumum heap
	for (i = 0; i < arrSize; i++)
	{
		vA.push_back(arr[i]);
		push_heap(vA, vA.size(), greater<int>());
		vB.push_back(arr[i]);
		push_heap(vB, vB.size(), less<int>());
	}

	// clear the heaps by popping elements. output the
	// optimum value which is located at back of vector
	cout << "Maximum heap: ";
	while (!vA.empty())
	{
		pop_heap(vA, vA.size(), greater<int>());
		cout << vA.back() << "  ";
		vA.pop_back();
	}
	cout << endl;

	cout << "Minimum heap: ";
	while (!vB.empty())
	{
		pop_heap(vB, vB.size(), less<int>());
		cout << vB.back() << "  ";
		vB.pop_back();
	}
	cout << endl;
	return 0;
}

/*
Run:

Maximum heap: 9  8  7  5  3  2  1
Minimum heap: 1  2  3  5  7  8  9
*/
