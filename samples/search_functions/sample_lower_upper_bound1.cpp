// lower_bound/upper_bound example
#include <iostream>     // std::cout
#include <algorithm>    // std::lower_bound, std::upper_bound, std::sort
#include <vector>       // std::vector
#include "search_functions.hpp"

int main () {
  std::vector<int> v= {10,20,30,30,20,10,10,20}; // 10 20 30 30 20 10 10 20

  std::sort (v.begin(), v.end());                // 10 10 10 20 20 20 30 30

  int low,up;
  low=MiniUtils::lower_bound (v, 20);            //          ^
  up= MiniUtils::upper_bound (v, 20);            //                   ^

  std::cout << "lower_bound at position " << low << '\n';
  std::cout << "upper_bound at position " << up << '\n';

  return 0;
}
