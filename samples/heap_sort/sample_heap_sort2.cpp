/** \example heap_sort/sample_heap_sort2.cpp
 * This is an example of how to use the heap_sort function.
 */
#include <iostream>
#include <vector>
#include <functional>
#include <random>

#include "heap_sort.hpp"

using namespace std;
using namespace MiniUtils;

int main()
{
	// objects to store the elements and generate random numbers
	vector<int> v;
    std::default_random_engine dre;
    std::uniform_int_distribution<int> di(0, 99);  // random in [0, 99]
	int i;

	// create a vector with 15 random integers
	for (i = 0; i < 15; i++)
		v.push_back(di(dre));

	// display the original list
	cout << "Original vector" << endl << "   ";
    for (auto item: v)
        cout << item << " ";
	cout << endl;

	// call heapSort() with greater<int>() and display results
	cout << "Sort in ascending order" << endl << "   ";
	heap_sort(v,greater<int>());
    for (auto item: v)
        cout << item << " ";
	cout << endl;

	// call heapSort() with less<int>() and display results
	cout << "Sort in descending order" << endl << "   ";
	heap_sort(v,less<int>());
    for (auto item: v)
        cout << item << " ";
	cout << endl;

	return 0;
}

/*
Run:

Original vector
   92  77  88  36  19  34  14  49  61  14  1  52  44  77  79

Sort in ascending order
   1  14  14  19  34  36  44  49  52  61  77  77  79  88  92

Sort in descending order
   92  88  79  77  77  61  52  49  44  36  34  19  14  14  1
*/
