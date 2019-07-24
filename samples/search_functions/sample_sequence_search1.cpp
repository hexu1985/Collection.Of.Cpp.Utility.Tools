/** \example search_functions/sample_sequence_search1.cpp
 * This is an example of how to use the sequence_search function.
 */
#include <iostream>     // std::cout
#include <vector>       // std::vector
#include "search_functions.hpp"

using namespace MiniUtils;

int main () {
  // using std::find with array and pointer:
  std::vector<int> myvector = { 10, 20, 30, 40 };

  // using std::find with vector and iterator:
  int i;
    
  i = sequence_search (myvector, 30);
  if (i != myvector.size())
    std::cout << "Element found in myvector: " << myvector[i] << '\n';
  else
    std::cout << "Element not found in myvector\n";

  return 0;
}

/*
Output:
Element found in myints: 30
Element found in myvector: 30
*/
