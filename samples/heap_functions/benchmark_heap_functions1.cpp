/** \example heap_functions/sample_heap_functions1.cpp
 * This is an example of how to use the heap functions.
 */
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
	int arr[] = {5, 9, 2, 7, 1, 3, 8};
	vector<int> vA, vB;

	// load arr element in vA as a maximum heap and
	// in vB as a minumum heap
	for (const auto &item: arr)
	{
		vA.push_back(item);
		push_heap(vA.begin(), vA.end(), less<int>());
		vB.push_back(item);
		push_heap(vB.begin(), vB.end(), greater<int>());
	}

	// clear the heaps by popping elements. output the
	// optimum value which is located at back of vector
	cout << "Maximum heap: ";
	while (!vA.empty())
	{
		pop_heap(vA.begin(), vA.end(), less<int>());
		cout << vA.back() << "  ";
		vA.pop_back();
	}
	cout << endl;

	cout << "Minimum heap: ";
	while (!vB.empty())
	{
		pop_heap(vB.begin(), vB.end(), greater<int>());
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
