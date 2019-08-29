/** \example random_number/sample_rand_int1.cpp
 * This is an example of how to use the RandomNumber class
 * to generate random int.
 */
#include <iostream>
#include <cassert>
#include "RandomNumber.hpp"

using namespace mini_utils;

int main()
{
  const int nrolls = 10000; // number of experiments
  const int nstars = 95;    // maximum number of stars to distribute

  RandomNumber random;

  int p[10]={};

  for (int i=0; i<nrolls; ++i) {
    int number = random.randInt(0, 9);
    assert(number >= 0); assert(number <= 9);
    ++p[number];
  }

  std::cout << "uniform_int_distribution (0,9):" << std::endl;
  for (int i=0; i<10; ++i)
    std::cout << i << ": " << std::string(p[i]*nstars/nrolls,'*') << std::endl;

  return 0;
}

/*
Possible output:
uniform_int_distribution (0,9):
0: *********
1: *********
2: *********
3: *********
4: *********
5: *********
6: *********
7: *********
8: *********
9: *********
*/
