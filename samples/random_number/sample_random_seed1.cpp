/* srand example */
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
