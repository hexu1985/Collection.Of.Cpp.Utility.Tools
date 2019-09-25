
#ifndef MINI_UTILS_RADIX_SORT_INC
#define MINI_UTILS_RADIX_SORT_INC

#include <vector>
#include <queue>

namespace mini_utils {

// support function for radix_sort()
// distribute vector elements into one of 10 queues
// using the digit corresponding to power
//   power = 1    ==> 1's digit
//   power = 10   ==> 10's digit
//   power = 100  ==> 100's digit
//   ...
template <typename Integer>
void distribute(const std::vector<Integer>& v, std::queue<Integer> digitQueue[],
                int power)
{
	int i;

	// loop through the vector, inserting each element into
	// the queue (v[i] / power) % 10
	for (i = 0; i < v.size(); i++)
		digitQueue[(v[i] / power) % 10].push(v[i]);
}

// support function for radix_sort()
// gather elements from the queues and copy back to the vector
template <typename Integer>
void collect(std::queue<Integer> digitQueue[], std::vector<Integer>& v)
{
	int i = 0, digit;

	// scan the vector of queues using indices 0, 1, 2, etc.
	for (digit = 0; digit < 10; digit++)
		// collect items until queue empty and copy items back
		// to the vector
		while (!digitQueue[digit].empty())
		{
			v[i] = digitQueue[digit].front();
			digitQueue[digit].pop();
			i++;
		}
}

template <typename Integer>
void radix_sort(std::vector<Integer>& v, int d)
{
	int i;
	int power = 1;
	std::queue<Integer> digitQueue[10];

	for (i=0;i < d;i++)
	{
		distribute(v, digitQueue, power);
		collect(digitQueue, v);
		power *= 10;
	}
}

}   // namespace mini_utils

#endif
