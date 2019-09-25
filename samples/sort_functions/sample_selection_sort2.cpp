// sort algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include <cassert>

#include "sort_functions.hpp"

using namespace mini_utils;

// 以编译期常量形式返回数组尺寸
// (该数组形参未起名字, 因为我们只关心其含有的元素个数)
template <typename T, std::size_t N>
constexpr std::size_t array_size(T (&)[N]) noexcept
{
    return N;
}

int main () {
  int myvector[] = {32,71,12,45,26,80,53,33};               // 32 71 12 45 26 80 53 33

  // using object as comp
  selection_sort (myvector, array_size(myvector));  		//(12 26 32 33 45 53 71 80)

  // print out content:
  std::cout << "myvector contains:";
  for (const auto &item: myvector)
    std::cout << ' ' << item;
  std::cout << '\n';

  assert(std::is_sorted(myvector, myvector+array_size(my_vector)));

  return 0;
}

/*
Output:
myvector contains: 12 26 32 33 45 53 71 80
*/
