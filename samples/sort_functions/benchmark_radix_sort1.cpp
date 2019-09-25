// File: prg8_2.cpp
// the program initializes a 50-element vector with 5-digit random
// integers in the range 0 to 99999 and sorts the vector using the
// radix sort. it outputs the numbers in 6 columns using the
// function display_vector()

#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

// output v, 6 elements per line
void display_vector(const vector<int>& v);

int main()
{
	// vector to hold the data that is sorted
	vector<int> intVector;
    std::default_random_engine generator;

	int i;

	// initialize vector with 50 random numbers in range 0 - 99999
    std::uniform_int_distribution<int> distribution(0,99999);
	for (i = 0; i < 50; i++)
		intVector.push_back(distribution(generator));

	// apply the radix sort and output the sorted vector
    std::sort(intVector.begin(), intVector.end());
	display_vector(intVector);

	return 0;
}

void display_vector(const vector<int>& v)
{
	int i;

	for (i=0; i < v.size(); i++)
	{
		// output each element in 12 spaces
		cout << setw(12) << v[i];
		if ((i+1) % 6 == 0)		// newline every 6 numbers
			cout << endl;
	}
	cout << endl;
}

/*
Run:
        3930        6921        7801       10277       10680       12227
       12850       15897       16273       17178       18226       18702
       20133       20373       24554       26389       26988       27395
       30359       32636       35678       39582       41033       41385
       42447       47526       48484       53883       55710       59465
       59675       60133       64623       68462       70153       71142
       71296       75233       76463       82925       83973       85099
       86845       87009       89763       89832       91424       92877
       94422       95966
*/
