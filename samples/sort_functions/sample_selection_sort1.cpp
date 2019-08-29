// sort algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include <cassert>

#include "sort_functions.hpp"

using namespace mini_utils;

int main () {
  std::vector<int> myvector = {32,71,12,45,26,80,53,33};      // 32 71 12 45 26 80 53 33

  // using object as comp
  selection_sort (myvector);  		                          //(12 26 32 33 45 53 71 80)

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
