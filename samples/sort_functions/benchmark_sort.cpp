/** \example sort_functions/benchmark_sort.cpp
 * This is an example of how to use the std::sort function.
 */
#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include <cassert>

int main () {
  std::vector<int> myvector = {32,71,12,45,26,80,53,33};      // 32 71 12 45 26 80 53 33

  // using object as comp
  std::sort (myvector.begin(), myvector.end());     		  //(12 26 32 33 45 53 71 80)

  // print out content:
  std::cout << "myvector contains:";
  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  assert(std::is_sorted(myvector.begin(), myvector.end()));

  return 0;
}

/*
Output:
myvector contains: 12 26 32 33 45 53 71 80
*/
