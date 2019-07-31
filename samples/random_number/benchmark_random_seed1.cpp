/** \example random_number/benchmark_random_seed1.cpp
 * This is an example of how to use the rand and srand
 * to generate random int.
 */
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

int main ()
{
  printf ("First number: %d\n", rand()%100);
  srand (time(NULL));
  printf ("Random number: %d\n", rand()%100);
  srand (1);
  printf ("Again the first number: %d\n", rand()%100);

  return 0;
}

/*
Possible output:

First number: 41
Random number: 13
Again the first number: 41
*/
