#pragma once

#include <iostream>
#include <gsl/span>

namespace recipe_6_10
{
   void func(int* buffer, size_t length) { /* ? */ }

   void func(gsl::span<int> buffer)
   {
      for (auto const e : buffer)
         std::cout << e << ' ';
      std::cout << '\n';
   }
   void execute()
   {
      {
         int arr[] = { 1, 1, 2, 3, 5, 8, 13 };
         gsl::span<int, 7> s{ arr };
      }

      {
         int arr[] = { 1, 1, 2, 3, 5, 8, 13 };
         gsl::span<int> s{ arr };
      }

      {
         int arr[] = { 1, 1, 2, 3, 5, 8, 13 };
         gsl::span<int, 7> s{ arr };
         std::cout << s.front() << " == " << s[0] << '\n';            // 1 == 1
         std::cout << s.back() << " == " << s[s.size() - 1] << '\n'; // 13 == 13
      }

      {
         int arr[] = { 1, 1, 2, 3, 5, 8, 13 };
         gsl::span<int, 7> s{ arr };
         gsl::span<int> first_3 = s.first(3);
         func(first_3);  // 1 1 2
         gsl::span<int> last_3 = s.last(3);
         func(last_3);   // 5 8 13
         gsl::span<int> mid_3 = s.subspan(2, 3);
         func(mid_3);    // 2 3 5
      }
   }
}
