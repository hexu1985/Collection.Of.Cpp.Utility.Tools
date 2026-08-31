#pragma once

#include <vector>
#include <string>


#ifdef _WIN32
  #define HELLO_EXPORT __declspec(dllexport)
#else
  #define HELLO_EXPORT
#endif

HELLO_EXPORT void hello();
HELLO_EXPORT void hello_print_vector(const std::vector<std::string> &strings);
