/** \example random_number/sample_frandom1.cpp
 * This is an example of how to use the RandomNumber class
 * to generate random double between 0 and 1 (include 0 and exclude 1).
 */
#include <iostream>
#include <cassert>
#include "RandomNumber.hpp"

using namespace mini_utils;

int main()
{
  const int nrolls=10000;  // number of experiments
  const int nstars=95;     // maximum number of stars to distribute
  const int nintervals=10; // number of intervals

  RandomNumber random;

  int p[nintervals]={};

  for (int i=0; i<nrolls; ++i) {
    double number = random.frandom();
    assert(number >= 0.0); assert(number < 1.0);
    ++p[int(nintervals*number)];
  }

  std::cout << "uniform_real_distribution (0.0,1.0):" << std::endl;
  std::cout << std::fixed; std::cout.precision(1);

  for (int i=0; i<nintervals; ++i) {
    std::cout << float(i)/nintervals << "-" << float(i+1)/nintervals << ": ";
    std::cout << std::string(p[i]*nstars/nrolls,'*') << std::endl;
  }

  return 0;
}

/*
Possible output:
uniform_real_distribution (0.0,1.0):
0.0-0.1: *********
0.1-0.2: *********
0.2-0.3: *********
0.3-0.4: *********
0.4-0.5: *********
0.5-0.6: *********
0.6-0.7: *********
0.7-0.8: *********
0.8-0.9: *********
0.9-1.0: *********
*/
