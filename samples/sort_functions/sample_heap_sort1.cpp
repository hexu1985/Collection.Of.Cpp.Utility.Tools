/** \example sort_functions/sample_heap_sort1.cpp
 * This is an example of how to use the heap_sort function.
 */
#include <iostream>
#include <vector>
#include <functional>
#include <random>

#include "sort_functions.hpp"

using namespace std;
using namespace mini_utils;

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
    for (auto &item: v) {
        cout << item << " ";
    }
	cout << endl;

	// call heapSort() with greater<int>() and display results
	cout << "Sorted vector" << endl << "   ";
	heap_sort(v);
    for (auto &item: v) {
        cout << item << " ";
    }
	cout << endl;

	return 0;
}

/*
Run:

Original vector
   92  77  88  36  19  34  14  49  61  14  1  52  44  77  79

Sorted vector
   1  14  14  19  34  36  44  49  52  61  77  77  79  88  92
*/
