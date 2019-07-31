/** \example random_number/sample_random_seed1.cpp
 * This is an example of how to use the Random class's random and seed method
 * to generate random int.
 */
#include <stdio.h>      /* printf, NULL */
#include <time.h>       /* time */
#include "RandomNumber.hpp"

using namespace MiniUtils;

int main ()
{
  RandomNumber random;
  printf ("First number: %d\n", random.random()%100);
  random.seed (time(NULL));
  printf ("Random number: %d\n", random.random()%100);
  random.seed (1);
  printf ("Again the first number: %d\n", random.random()%100);

  return 0;
}

/*
Possible output:

First number: 41
Random number: 13
Again the first number: 41
*/
