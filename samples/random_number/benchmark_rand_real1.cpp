/** \example random_number/benchmark_rand_real1.cpp
 * This is an example of how to use the std::default_random_engine and std::uniform_real_distribution<double> 
 * to generate random double.
 */
#include <iostream>
#include <random>
#include <cassert>

int main()
{
  const int nrolls=10000;  // number of experiments
  const int nstars=95;     // maximum number of stars to distribute
  const int nintervals=10; // number of intervals

  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(0.0,1.0);

  int p[nintervals]={};

  for (int i=0; i<nrolls; ++i) {
    double number = distribution(generator);
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
