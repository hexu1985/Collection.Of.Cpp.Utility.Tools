// set_difference example
#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include "set_functions.hpp"

using namespace mini_utils;

int main () {
  std::vector<int> first = {5,10,15,20,25};
  std::vector<int> second = {50,40,30,20,10};

  std::sort (first.begin(),first.end());     //  5 10 15 20 25
  std::sort (second.begin(),second.end());   // 10 20 30 40 50

  auto v = set_difference (first, second);

  std::cout << "The difference has " << (v.size()) << " elements:\n";
  for (auto it=v.begin(); it!=v.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

	

/*
Output:

The difference has 3 elements:
 5 15 25
*/
